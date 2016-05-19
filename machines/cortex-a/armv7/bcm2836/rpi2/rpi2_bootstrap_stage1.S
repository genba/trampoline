
.extern __SP_fiq_
.extern __SP_irq_
.extern __SP_mon_
.extern __SP_und_
.extern __SP_abt_
.extern __SP_svc_
.extern __SP_hyp_
.extern __SP_usr_

#define ARMV7_CP15_C0_MPID_MPE               (1 << 31)
#define ARMV7_CP15_C0_MPID_U_UNIPROCESSOR    (1 << 30)
#define ARMV7_CP15_C0_MPID_U_MULTIPROCESSOR  (0 << 30)
#define ARMV7_CP15_C0_MPID_CLUSTERID_MASK    (0xF << 8)
#define ARMV7_CP15_C0_MPID_CPUID_MASK        (3 << 0)

.section .startup_code, "ax"
.global _start
_start:
  @Save parameters
  @r0 - Zero
  @r1 - Machine Type (Raspberry Pi 2 or BCM2709 = 0x0C42)
  @r2 - Address of the ARM Tags structure (Normally 0x0100)
  @ldr r3, .LARMTagsAddress
  @cmp r2, #0
  @moveq r2, #0x0100
  @str r2, [r3]

  @Save the ARM Machine Type that the bootloader should have passed to us
  @in R1. See: http://www.arm.linux.org.uk/developer/machines/
  @ldr r3, .LARMMachineType
  @str r1, [r3]

  @Save the ARM Boot Mode that the CPU was in at startup
  ldr r3, StartupBootMode
  mrs r0, cpsr
  and r0, r0, #0x0000001F @#ARM_MODE_BITS mask
  str r0, [r3]

  @Save the Vector Base Address that was current at startup
  ldr r3, StartupBootVectors
  mrc p15, #0, r0, cr12, cr0, #0
  str r0, [r3]

  /*
   * Continue execution at the StartupHandler.
   */
  b startup

  /*
   * Holds the CPU startup mode
   */
StartupBootMode:
  .long 0x0
  /*
   * Holds the Vector Base Address on startup.
   */
StartupBootVectors:
  .long 0x0
  /*
   * Holds the Vector Base Address on startup.
   */
/*.LARMTagsAddress:
  .long ARMTagsAddress
.LARMMachineType:
  .long ARMMachineType*/
  @Test initial mode whether HYP or SUP

  @Initialize the micro according to the detected initial mode

  @b startup
_start_end:

@----------------------------------------------------------------
startup:
  bl startupSwitchMode

  ldr r0, =__SP_svc_
  msr SP_svc, r0

  bl startupSecureMode
  @Invalidate all Caches before starting the boot process
  bl ARMv7InvalidateCache
  @Invalidate the TLB before starting the boot process
  bl ARMv7InvalidateTLB
  @Change to SYS mode and ensure all interrupts are disabled
  @so the ARM processor is in a known state.
  bl setupStacks
  bl switchToSysMode
  @Disable interrupts and switch to SYS mode
  cpsid if, #0x1F @#ARM_MODE_SYS

  @Copy the ARM exception table from Vectors to the vector base address.
  mov r0, #0x00001000 @#RPI2_VECTOR_TABLE_BASE
  ldr r1, =vectors
  ldmia r1!, {r2-r9}
  stmia r0!, {r2-r9}
  ldmia r1!, {r2-r9}
  stmia r0!, {r2-r9}
  @Set the Vector Base Address register in the System Control
  @register to the address of the vector table base above.
  mov r0, #0x00001000 @#RPI2_VECTOR_TABLE_BASE
  mcr p15, #0, r0, cr12, cr0, #0


  bl tpl_armv7_bootstrap_stage2
startup_end:

@----------------------------------------------------------------
setupStacks:
  @ldr r0, =__SP_fiq_
  @msr SP_fiq, r0
  ldr r0, =__SP_irq_
  msr SP_irq, r0
  @ldr r0, =__SP_mon_
  @msr SP_mon, r0
  @ldr r0, =__SP_und_
  @msr SP_und, r0
  @ldr r0, =__SP_abt_
  @msr SP_abt, r0
  @ La pile pour le mode Superviseur est positionnée avant
  ldr r0, =__SP_svc_
  msr SP_svc, r0
  @ldr r0, =__SP_hyp_
  @msr SP_hyp, r0
  ldr r0, =__SP_usr_
  msr SP_usr, r0
setupStacks_end:

@----------------------------------------------------------------
startupSwitchMode:
  @Get the CPSR
  mrs r0, cpsr
  @Test for HYP mode
  eor r0, r0, #0x1A @#ARM_MODE_HYP
  tst r0, #0x1F @#ARM_MODE_BITS
  @Clear the mode bits
  bic r0, r0, #0x1F @#ARM_MODE_BITS
  @Mask IRQ/FIQ bits and set SVC mode
  orr r0, r0, #(0x80 | 0x40 | 0x13) @#(ARM_I_BIT | ARM_F_BIT | ARM_MODE_SVC)

  @Return if not in HYP mode
  bne startupSwitchMode_noSwitch

  @Mask the Abort bit
  orr r0, r0, #0x100 @#ARM_A_BIT
  @Load the SPSR
  msr spsr_cxsf, r0
  @Return to SVC mode
  msr ELR_hyp, lr
  eret
startupSwitchMode_noSwitch:
  @Set the CPSR (C fields)
  msr cpsr_c, r0
  @Return to startup
  bx lr
startupSwitchMode_end:

@----------------------------------------------------------------
startupSecureMode:
  @Check the secure boot configuration
  mov r0, #0x1 @#RPI2_SECURE_BOOT
  cmp r0, #0
  beq startupSecureMode_noSecure
  @Attempt to switch back to secure world by performing a
  @secure monitor call to the Secure Monitor handler.
  @Get the vector base address from the System Control register.
  mrc p15, #0, r0, cr12, cr0, #0
  @Get the address of the secure vectors
  ldr r1, =secureVectors
  @Copy the secure vector table
  ldmia r1!, {r2-r9}
  stmia r0!, {r2-r9}
  ldmia r1!, {r2-r9}
  stmia r0!, {r2-r9}
  @Clean Data Cache MVA
  mov r12, #0
  mcr p15, #0, r12, cr7, cr10, #1
  @Perform a data synchronisation barrier
  dsb
  @Invalidate Instruction Cache
  mov r12, #0
  mcr p15, #0, r12, cr7, cr5, #0
  @Flush Branch Target Cache
  mov r12, #0
  mcr p15, #0, r12, cr7, cr5, #6
  @Perform a data synchronisation barrier
  dsb
  @Perform an instruction synchronisation barrier
  isb
  @Perform a secure monitor call (Not supported by the FPC compiler)
  smc #0
startupSecureMode_noSecure:
  @Return to startup
  bx lr
startupSecureMode_end:

@----------------------------------------------------------------
switchToSysMode:
  @Disable interrupts and switch to SYS mode
  cpsid if, #0x1F @#ARM_MODE_SYS

  @Copy the ARM exception table from Vectors to the vector base address.
  mov r0, #0x00001000 @#RPI2_VECTOR_TABLE_BASE
  ldr r1, =vectors
  ldmia r1!, {r2-r9}
  stmia r0!, {r2-r9}
  ldmia r1!, {r2-r9}
  stmia r0!, {r2-r9}
  @Set the Vector Base Address register in the System Control
  @register to the address of the vector table base above.
  mov r0, #0x00001000 @#RPI2_VECTOR_TABLE_BASE
  mcr p15, #0, r0, cr12, cr0, #0

  bx lr
switchToSysMode_end:

@----------------------------------------------------------------
switchToUserMode:
  cpsid if
  cps #0x1F @#ARM_MODE_USR
switchToUserMode_end:


@----------------------------------------------------------------
.global ARMv7InvalidateCache
ARMv7InvalidateCache:
  @Get the MPID register from the system control coprocessor CP15
  mrc p15, #0, r0, cr0, cr0, #5

  @Mask off the Multiprocessor Extensions bit (MPE)
  and r1, r0, #ARMV7_CP15_C0_MPID_MPE
  cmp r1, #0
  beq ARMv7InvalidateCache_LUniprocessor

  @Mask off the Uniprocessor bit (U)
  and r1, r0, #ARMV7_CP15_C0_MPID_U_UNIPROCESSOR
  cmp r1, #0
  bne ARMv7InvalidateCache_LUniprocessor

  @Invalidate all instruction caches to PoU (Inner Shareable)
  mov r12, #0
  mcr p15, #0, r12, cr7, cr1, #0

  @Perform a data synchronization barrier
  dsb

  @Branch to ARMv7InvalidateDataCache (Will return to caller via LR)
  b ARMv7InvalidateDataCache

ARMv7InvalidateCache_LUniprocessor:
  @Invalidate all instruction caches to PoU
  mov r12, #0
  mcr p15, #0, r12, cr7, cr5, #0

  @Perform a data synchronization barrier
  dsb

  @Branch to ARMv7InvalidateDataCache (Will return to caller via LR)
  b ARMv7InvalidateDataCache
ARMv7InvalidateCache_end:

@----------------------------------------------------------------
@ Perform an invalidate entire TLB operation using the c8 (TLB Operations) register of system control coprocessor CP15
@ See page B3-138 of the ARMv7 Architecture Reference Manual
.global ARMv7InvalidateTLB
ARMv7InvalidateTLB:
  @Get the MPID register from the system control coprocessor CP15
  mrc p15, #0, r0, cr0, cr0, #5

  @Mask off the Multiprocessor Extensions bit (MPE)
  and r1, r0, #ARMV7_CP15_C0_MPID_MPE
  cmp r1, #0
  beq ARMv7InvalidateTLB_LUniprocessor

  @Mask off the Uniprocessor bit (U)
  and r1, r0, #ARMV7_CP15_C0_MPID_U_UNIPROCESSOR
  cmp r1, #0
  bne ARMv7InvalidateTLB_LUniprocessor

  @Invalidate entire TLB (Unified/Inner Shareable)
  mov r12, #0
  mcr p15, #0, r12, cr8, cr3, #0

  @Perform a data synchronization barrier
  dsb

  @Return to caller
  bx lr

ARMv7InvalidateTLB_LUniprocessor:
  @Invalidate entire TLB (Unlocked/Unified)
  mov r12, #0
  mcr p15, #0, r12, cr8, cr7, #0

  @Perform a data synchronization barrier
  dsb
ARMv7InvalidateTLB_end:

@----------------------------------------------------------------
@ Perform an invalidate entire data cache operation
@ See page B3-127 of the ARMv7 Architecture Reference Manual

ARMv7InvalidateDataCache:
  mrc	p15, #1, r0, cr0, cr0, #1        @read CLIDR
  tst	r0, #0x07000000
  beq	ARMv7InvalidateDataCache_LDone
  mov	r3, #0			                 @start with L1

ARMv7InvalidateDataCache_LStart:
 add	r2, r3, r3, lsr #1	             @r2 = level * 3 / 2
 mov	r1, r0, lsr r2		             @r1 = cache type
 tst	r1, #6			                 @is it unified or data?
 beq	ARMv7InvalidateDataCache_LNext	                         @nope, skip level

 mcr	p15, #2, r3, cr0, cr0, #0	     @select cache level
 isb
 mrc	p15, #1, r0, cr0, cr0, #0	     @read CCSIDR

 ubfx	ip, r0, #0, #3		             @get linesize from CCSIDR
 add	ip, ip, #4		                 @apply bias
 ubfx	r2, r0, #13, #15	             @get numsets - 1 from CCSIDR
 lsl	r2, r2, ip		                 @shift to set position
 orr	r3, r3, r2		                 @merge set into way/set/level
 mov	r1, #1
 lsl	r1, r1, ip		                 @r1 = set decr

 ubfx	ip, r0, #3, #10		             @get numways - 1 from [to be discarded] CCSIDR
 clz	r2, ip			                 @number of bits to MSB of way
 lsl	ip, ip, r2		                 @shift by that into way position
 mov	r0, #1
 lsl	r2, r0, r2		                 @r2 now contains the way decr
 mov	r0, r3 			                 @get sets/level (no way yet)
 orr	r3, r3, ip		                 @merge way into way/set/level
 bfc	r0, #0, #4		                 @clear low 4 bits (level) to get numset - 1
 sub	r2, r2, r0		                 @subtract from way decr

 @r3 = ways/sets/level, r2 = way decr, r1 = set decr, r0 and ip are free
ARMv7InvalidateDataCache_LLoop:
 mcr	p15, #0, r3, cr7, cr6, #2	     @invalidate line
 cmp	r3, #15			                 @are we done with this level (way/set == 0)
 bls	ARMv7InvalidateDataCache_LNext	                         @yes, go to next level
 ubfx	r0, r3, #4, #18		             @extract set bits
 cmp	r0, #0			                 @compare
 subne	r3, r3, r1		                 @non-zero?, decrement set #
 subeq	r3, r3, r2		                 @zero?, decrement way # and restore set count
 b	ARMv7InvalidateDataCache_LLoop

ARMv7InvalidateDataCache_LNext:
 dsb
 mrc	p15, #1, r0, cr0, cr0, #1	     @read CLIDR
 ubfx	ip, r0, #24, #3		             @narrow to LoC
 add	r3, r3, #2		                 @go to next level
 cmp	r3, ip, lsl #1		             @compare
 blt	ARMv7InvalidateDataCache_LStart		                     @not done, next level (r0 == CLIDR)

ARMv7InvalidateDataCache_LDone:
 mov	r0, #0			                 @default back to cache level 0
 mcr	p15, #2, r0, cr0, cr0, #0	     @select cache level
 dsb
 isb
ARMv7InvalidateDataCache_end:

@----------------------------------------------------------------
.global reset_handler
reset_handler:
  bx lr
reset_handler_end:

@----------------------------------------------------------------
.global undefined_handler
undefined_handler:
  push {r0-r12, lr}
  bl c_undefined_handler
  pop {r0-r12, lr}
  subs pc,lr,#4
undefined_handler_end:

@----------------------------------------------------------------
.global prefetch_handler
prefetch_handler:
  push {r0-r12, lr}
  bl c_prefetch_handler
  pop {r0-r12, lr}
  subs pc,lr,#4
prefetch_handler_end:

@----------------------------------------------------------------
.global data_handler
data_handler:
  push {r0-r12, lr}
  bl c_data_handler
  pop {r0-r12, lr}
  subs pc,lr,#8
data_handler_end:

@----------------------------------------------------------------
.global reserved_handler
reserved_handler:
  push {r0-r12, lr}
  bl c_reserved_handler
  pop {r0-r12, lr}
  subs pc,lr,#4
reserved_handler_end:

@----------------------------------------------------------------
.global fiq_handler
fiq_handler:
  push {r0-r12, lr}
  bl c_fiq_handler
  pop {r0-r12, lr}
  subs pc,lr,#4
fiq_handler_end:

@----------------------------------------------------------------
.global smc_handler
smc_handler:
  mrc p15, 0, r1, c1, c1, 0
  bic r1, r1, #1
  mcr p15, 0, r1, c1, c1, 0
  movs pc, lr
smc_handler_end:

@----------------------------------------------------------------
.global dummy
dummy:
  bx lr

@----------------------------------------------------------------
.global enable_irq
enable_irq:
  mrs r0,cpsr
  bic r0,r0,#0x80
  msr cpsr_c,r0
  bx lr
enable_irq_end:

@----------------------------------------------------------------
@.global irq_handler
@irq_handler:
@  push {r0-r12, lr}
@  bl c_irq_handler
@  pop {r0-r12, lr}
@  @
@  @push {r0-r12, lr}
@  @bl trace_2
@  @pop {r0-r12, lr}
@
@  push {r0-r12, lr}
@  bl tpl_arm_subarch_irq_handler
@  pop {r0-r12, lr}
@
@  subs pc,lr,#4
@irq_handler_end:

.global svc_handler
svc_handler:
  @srsdb #0x1F! @#ARM_MODE_SYS!
  @cps #0x1F @#ARM_MODE_SYS
  @@Save all of the general registers (R0 to R12) and the SYS mode link register (LR)
  push {r0-r12, lr}
  @cps #0x13 @#ARM_MODE_SVC
  @
  @@Get the current CPU
  @@Read the Multiprocessor Affinity (MPIDR) register from the system control coprocessor CP15
  @@mrc p15, #0, r4, cr0, cr0, #5;
  @@Mask off the CPUID value
  @@and r4, #3 @#ARMV7_CP15_C0_MPID_CPUID_MASK
  @@Multiply by 4 to get the offset into the array
  @@lsl r6, r4, #2
  @
  @@Get the SWI thread id
  @@ldr r5, .LSWI_THREAD_HANDLE
  @@ldr r5, [r5]
  @@ldr r5, [r5, r6]
  @
  @bl c_svc_handler
  bl tpl_primary_syscall_handler
  @cps #0x13 @#ARM_MODE_SYS
  pop {r0-r12, lr}
  @rfeia sp!
  subs pc,lr,#0
svc_handler_end:


.global GETR0
GETR0:
  mov r0, r0
  bx lr
.global GETR1
GETR1:
  mov r0, r1
  bx lr
.global GETR2
GETR2:
  mov r0, r2
  bx lr
.global GETR3
GETR3:
  mov r0, r3
  bx lr
.global GETR4
GETR4:
  mov r0, r4
  bx lr
.global GETR5
GETR5:
  mov r0, r5
  bx lr
.global GETR6
GETR6:
  mov r0, r6
  bx lr
.global GETR7
GETR7:
  mov r0, r7
  bx lr
.global GETR8
GETR8:
  mov r0, r8
  bx lr
.global GETR9
GETR9:
  mov r0, r9
  bx lr
.global GETR10
GETR10:
  mov r0, r10
  bx lr
.global GETR11
GETR11:
  mov r0, r11
  bx lr
.global GETR12
GETR12:
  mov r0, r12
  bx lr
.global GETSP
GETSP:
  mov r0,sp
  bx lr
.global GETLR
GETLR:
  mov r0, lr
  bx lr
.global GETPC
GETPC:
  mov r0, pc
  bx lr

.global GETSP_usr
GETSP_usr:
  mrs r0,SP_usr
  bx lr

.global GETSP_irq
GETSP_irq:
  mrs r0,SP_irq
  bx lr

.global GETSPIRQ
GETSPIRQ:
  mrs r0, SP_irq
  bx lr

.global GETSPSVC
GETSPSVC:
  mrs r0, SP_svc
  bx lr

.global GETCPSR
GETCPSR:
  mrs r0,cpsr
  bx lr

.global GETISR
GETISR:
  mrc p15, 0, r0, c12, c1, 0
  bx lr

.global GETVBAR
GETVBAR:
  mrc p15, 0, r0, c12, c0, 0 ;@ get vbar
  bx lr