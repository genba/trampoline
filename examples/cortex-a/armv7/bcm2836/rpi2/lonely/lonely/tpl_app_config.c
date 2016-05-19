/**
 * @file tpl_app_config.c
 *
 * @section desc File description
 *
 * OS data structure generated from application lonely
 * Automatically generated by goil on Fri Apr  1 15:02:06 2016
 * from root OIL file lonely.oil
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005-2007
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#include "tpl_app_config.h"

#include "tpl_os_internal_types.h"
#include "tpl_machine.h"
#include "tpl_os_interrupt.h"
#include "tpl_os_interrupt_kernel.h"
#include "tpl_os_alarm_kernel.h"
#include "tpl_os_alarm.h"
#include "tpl_os_resource_kernel.h"
#include "tpl_os_resource.h"
#include "tpl_os_event_kernel.h"
#include "tpl_os_event.h"
#include "tpl_os_action.h"
#include "tpl_os_kernel.h"
#include "tpl_os_definitions.h"


/*=============================================================================
 * Application Modes tables for tasks, alarms and schedule tables
 * Application Modes masks are as follow:
 * std = 1
 */

CONST(tpl_application_mode, OS_CONST) std = 0; /* mask = 1 */
CONST(tpl_application_mode, OS_CONST) OSDEFAULTAPPMODE = 0;
CONST(tpl_appmode_mask, OS_CONST) tpl_task_app_mode[TASK_COUNT] = {
  1 /* task task1 : std */ ,
  0 /* task task2 :  */ ,
  0 /* task task3 :  */ 
};

CONST(tpl_appmode_mask, OS_CONST) tpl_alarm_app_mode[ALARM_COUNT] = {
  1 /* alarm Alarm1 : std */ 
};

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Declaration of resources IDs
 */

/* Resource lcd_resource */
#define lcd_resource_id 0
CONST(ResourceType, AUTOMATIC) lcd_resource = lcd_resource_id;

/*=============================================================================
 * Declaration of processes IDs
 */

/* Task task1 identifier */
#define task1_id 0
CONST(TaskType, AUTOMATIC) task1 = task1_id;

/* Task task2 identifier */
#define task2_id 1
CONST(TaskType, AUTOMATIC) task2 = task2_id;

/* Task task3 identifier */
#define task3_id 2
CONST(TaskType, AUTOMATIC) task3 = task3_id;

/* ISR isr_button2 identifier */
#define isr_button2_id 3
/* ISR isr_button1 identifier */
#define isr_button1_id 4
/*=============================================================================
 * Declaration of Alarm IDs
 */

/* Alarm Alarm1 identifier */
#define Alarm1_id 0
CONST(AlarmType, AUTOMATIC) Alarm1 = Alarm1_id;

#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Definition and initialization of regular Resource related structures
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*-----------------------------------------------------------------------------
 * Resource descriptor of resource lcd_resource
 *
 * Tasks which use this resource :
 * task1
 * task2
 * task3
 * ISRs which use this resource :
 * isr_button1
 * isr_button2
 */
VAR(tpl_resource, OS_VAR) lcd_resource_rez_desc = {
  /* ceiling priority of the resource */  5,
  /* owner previous priority          */  0,
  /* owner of the resource            */  INVALID_PROC_ID,
#if WITH_OSAPPLICATION == YES
  /* OS Application id                */  
#endif    
  /* next resource in the list        */  NULL
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2VAR(tpl_resource, AUTOMATIC, OS_APPL_DATA)
tpl_resource_table[RESOURCE_COUNT] = {
  &lcd_resource_rez_desc,
  &res_sched_rez_desc  
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Counters related defines and structures
 */
/*-----------------------------------------------------------------------------
 * Counter SystemCounter descriptor
 */

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_tick, OS_CONST) OSTICKSPERBASE = 10;
CONST(tpl_tick, OS_CONST) OSMAXALLOWEDVALUE = 2000;
CONST(tpl_tick, OS_CONST) OSMINCYCLE = 1;

#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_counter, OS_VAR) SystemCounter_counter_desc = {
  /* ticks per base       */  10,
  /* max allowed value    */  2000,
  /* minimum cycle        */  1,
  /* current tick         */  0,
  /* current date         */  0,
#if WITH_OSAPPLICATION == YES
    /* OS Application id    */  
#endif
    /* first alarm          */  NULL_PTR,
    /* next alarm to raise  */  NULL_PTR
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#include "tpl_os_kernel.h"          /* tpl_schedule */
#include "tpl_os_timeobj_kernel.h"  /* tpl_counter_tick */
#include "tpl_machine_interface.h"  /* tpl_switch_context_from_it */

#define OS_START_SEC_CODE
#include "tpl_memmap.h"

FUNC(void, OS_CODE) tpl_tick_it_timer1()
{
  tpl_counter_tick(&SystemCounter_counter_desc);

  
  if (tpl_kern.need_schedule)
  {
    tpl_schedule_from_running();
    LOCAL_SWITCH_CONTEXT()
  }
}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

/*=============================================================================
 * Descriptor(s) of the idle task(s)
 */
/*-----------------------------------------------------------------------------
 * Task IDLE_TASK descriptor
 */
#define OS_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * Task IDLE_TASK function prototype
 */

FUNC(void, OS_APPL_CODE) idle_function(void);
#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task IDLE_TASK stack
 */
#define OS_START_SEC_VAR_16BIT
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) IDLE_TASK_stack_zone[IDLE_STACK_SIZE/sizeof(tpl_stack_word)];
#define OS_STOP_SEC_VAR_16BIT
#include "tpl_memmap.h"

#define IDLE_TASK_STACK {IDLE_TASK_stack_zone, IDLE_STACK_SIZE}

/*
 * Task IDLE_TASK context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) IDLE_TASK_int_context;

#define IDLE_TASK_CONTEXT &IDLE_TASK_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"





/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of task IDLE_TASK
 */
CONST(tpl_proc_static, OS_CONST) IDLE_TASK_task_stat_desc = {
  /* context                  */  IDLE_TASK_CONTEXT,
  /* stack                    */  IDLE_TASK_STACK,
  /* entry point (function)   */  idle_function,
  /* internal ressource       */  NULL,
  /* task id                  */  IDLE_TASK_ID,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  0,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task IDLE_TASK
 */
VAR(tpl_proc, OS_VAR) IDLE_TASK_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  0,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Task related defines and structures
 */
/*-----------------------------------------------------------------------------
 * Task task1 descriptor
 */
#define APP_Task_task1_START_SEC_CODE

#include "tpl_memmap.h"
/*
 * Task task1 function prototype
 */

FUNC(void, OS_APPL_CODE) task1_function(void);
#define APP_Task_task1_STOP_SEC_CODE

#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task task1 stack
 */
#define APP_Task_task1_START_SEC_STACK

#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) task1_stack_zone[500/sizeof(tpl_stack_word)];
#define APP_Task_task1_STOP_SEC_STACK

#include "tpl_memmap.h"

#define task1_STACK {task1_stack_zone, 500}

/*
 * Task task1 context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) task1_int_context;

#define task1_CONTEXT &task1_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"





/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*
 * Resources used by task task1
 *
 * lcd_resource
 */

/*
 * Static descriptor of task task1
 */
CONST(tpl_proc_static, OS_CONST) task1_task_stat_desc = {
  /* context                  */  task1_CONTEXT,
  /* stack                    */  task1_STACK,
  /* entry point (function)   */  task1_function,
  /* internal ressource       */  NULL,
  /* task id                  */  task1_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  1,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task task1
 */
VAR(tpl_proc, OS_VAR) task1_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  1,
  /* task state                     */  AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Task task2 descriptor
 */
#define APP_Task_task2_START_SEC_CODE

#include "tpl_memmap.h"
/*
 * Task task2 function prototype
 */

FUNC(void, OS_APPL_CODE) task2_function(void);
#define APP_Task_task2_STOP_SEC_CODE

#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task task2 stack
 */
#define APP_Task_task2_START_SEC_STACK

#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) task2_stack_zone[500/sizeof(tpl_stack_word)];
#define APP_Task_task2_STOP_SEC_STACK

#include "tpl_memmap.h"

#define task2_STACK {task2_stack_zone, 500}

/*
 * Task task2 context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) task2_int_context;

#define task2_CONTEXT &task2_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"





/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*
 * Resources used by task task2
 *
 * lcd_resource
 */

/*
 * Static descriptor of task task2
 */
CONST(tpl_proc_static, OS_CONST) task2_task_stat_desc = {
  /* context                  */  task2_CONTEXT,
  /* stack                    */  task2_STACK,
  /* entry point (function)   */  task2_function,
  /* internal ressource       */  NULL,
  /* task id                  */  task2_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  2,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task task2
 */
VAR(tpl_proc, OS_VAR) task2_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  2,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Task task3 descriptor
 */
#define APP_Task_task3_START_SEC_CODE

#include "tpl_memmap.h"
/*
 * Task task3 function prototype
 */

FUNC(void, OS_APPL_CODE) task3_function(void);
#define APP_Task_task3_STOP_SEC_CODE

#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task task3 stack
 */
#define APP_Task_task3_START_SEC_STACK

#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) task3_stack_zone[500/sizeof(tpl_stack_word)];
#define APP_Task_task3_STOP_SEC_STACK

#include "tpl_memmap.h"

#define task3_STACK {task3_stack_zone, 500}

/*
 * Task task3 context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) task3_int_context;

#define task3_CONTEXT &task3_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"





/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*
 * Resources used by task task3
 *
 * lcd_resource
 */

/*
 * Static descriptor of task task3
 */
CONST(tpl_proc_static, OS_CONST) task3_task_stat_desc = {
  /* context                  */  task3_CONTEXT,
  /* stack                    */  task3_STACK,
  /* entry point (function)   */  task3_function,
  /* internal ressource       */  NULL,
  /* task id                  */  task3_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  3,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task task3
 */
VAR(tpl_proc, OS_VAR) task3_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  3,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of ISR2 related defines and structures
 */
/*-----------------------------------------------------------------------------
 * ISR isr_button2 descriptor
 */
#define APP_ISR_isr_button2_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * ISR isr_button2 function prototype
 */
FUNC(void, OS_APPL_CODE) isr_button2_function(void);
#define APP_ISR_isr_button2_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * ISR isr_button2 stack
 */
#define APP_ISR_isr_button2_START_SEC_STACK

#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) isr_button2_stack_zone[500/sizeof(tpl_stack_word)];
#define APP_ISR_isr_button2_STOP_SEC_STACK

#include "tpl_memmap.h"

#define isr_button2_STACK {isr_button2_stack_zone, 500}

/*
 * ISR isr_button2 context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) isr_button2_int_context;

#define isr_button2_CONTEXT &isr_button2_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"



/*
  No timing protection
 */

#if WITH_AUTOSAR_TIMING_PROTECTION == YES
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of ISR isr_button2
 */
CONST(tpl_proc_static, OS_CONST) isr_button2_isr_stat_desc = {
  /* context                  */  isr_button2_CONTEXT,
  /* stack                    */  isr_button2_STACK,
  /* entry point (function)   */  isr_button2_function,
  /* internal ressource       */  NULL,
  /* ISR id                   */  isr_button2_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif

  /* ISR base priority       */  5,
  /* ISR activation count     */  1,
  /* ISR type                */  IS_ROUTINE,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* pointer to the timing
     protection descriptor    */  $TIMING_PROT_REF$
#endif
};

CONST(tpl_isr_static, OS_CONST) isr_button2_isr_helper = {
  /* helper */ NULL,
  /* next */   NULL,
  /* id */     isr_button2_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of ISR isr_button2
 */
VAR(tpl_proc, OS_VAR) isr_button2_isr_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* ISR priority                   */  5,
  /* ISR state                      */  SUSPENDED,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* activation allowed             */  ,TRUE
#endif
};
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * ISR isr_button1 descriptor
 */
#define APP_ISR_isr_button1_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * ISR isr_button1 function prototype
 */
FUNC(void, OS_APPL_CODE) isr_button1_function(void);
#define APP_ISR_isr_button1_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * ISR isr_button1 stack
 */
#define APP_ISR_isr_button1_START_SEC_STACK

#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) isr_button1_stack_zone[500/sizeof(tpl_stack_word)];
#define APP_ISR_isr_button1_STOP_SEC_STACK

#include "tpl_memmap.h"

#define isr_button1_STACK {isr_button1_stack_zone, 500}

/*
 * ISR isr_button1 context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) isr_button1_int_context;

#define isr_button1_CONTEXT &isr_button1_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"



/*
  No timing protection
 */

#if WITH_AUTOSAR_TIMING_PROTECTION == YES
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of ISR isr_button1
 */
CONST(tpl_proc_static, OS_CONST) isr_button1_isr_stat_desc = {
  /* context                  */  isr_button1_CONTEXT,
  /* stack                    */  isr_button1_STACK,
  /* entry point (function)   */  isr_button1_function,
  /* internal ressource       */  NULL,
  /* ISR id                   */  isr_button1_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif

  /* ISR base priority       */  5,
  /* ISR activation count     */  1,
  /* ISR type                */  IS_ROUTINE,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* pointer to the timing
     protection descriptor    */  $TIMING_PROT_REF$
#endif
};

CONST(tpl_isr_static, OS_CONST) isr_button1_isr_helper = {
  /* helper */ NULL,
  /* next */   NULL,
  /* id */     isr_button1_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of ISR isr_button1
 */
VAR(tpl_proc, OS_VAR) isr_button1_isr_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* ISR priority                   */  5,
  /* ISR state                      */  SUSPENDED,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* activation allowed             */  ,TRUE
#endif
};
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#include "tpl_machine.h"


/* Functions prototypes */

/* Interrupt functions */

#define OS_START_SEC_CODE
#include "tpl_memmap.h"

FUNC(void, OS_CODE) tpl_interrupt_source_5()
{

  tpl_tick_it_timer1();
}

FUNC(void, OS_CODE) tpl_interrupt_source_14()
{

  tpl_central_interrupt_handler(4); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_16()
{

  tpl_central_interrupt_handler(3); 
}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"


/* Interrupt table vector */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONST(tpl_it_vector_entry, OS_CONST) tpl_it_vector[31] = {
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_interrupt_source_5, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_interrupt_source_14, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_interrupt_source_16, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL }
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
/*=============================================================================
 * Definition and initialization of process tables (tasks and isrs)
 */
CONSTP2CONST(tpl_proc_static, AUTOMATIC, OS_APPL_DATA)
tpl_stat_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &task1_task_stat_desc,
  &task2_task_stat_desc,
  &task3_task_stat_desc,
  &isr_button2_isr_stat_desc,
  &isr_button1_isr_stat_desc,
  &IDLE_TASK_task_stat_desc
};

CONSTP2VAR(tpl_proc, AUTOMATIC, OS_APPL_DATA)
tpl_dyn_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &task1_task_desc,
  &task2_task_desc,
  &task3_task_desc,
  &isr_button2_isr_desc,
  &isr_button1_isr_desc,
  &IDLE_TASK_task_desc
};

CONSTP2CONST(tpl_isr_static, AUTOMATIC, OS_APPL_DATA)
tpl_isr_stat_table[ISR_COUNT] = {
  &isr_button2_isr_helper,
  &isr_button1_isr_helper
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Alarm related structures
 */
/*-----------------------------------------------------------------------------
 * Alarm Alarm1 descriptor
 *
 * This alarm does the activation of task task2
 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_task_activation_action, OS_CONST) Alarm1_action = {
  {
    /* action function  */  tpl_action_activate_task
  },
  /* task id            */  task2_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
 
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_alarm_static, OS_CONST) Alarm1_static = {
  {
    /* pointer to counter           */  &SystemCounter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_TRACE == YES)
    /* id of the alarm for tracing  */  , Alarm1_id
#endif
#if WITH_OSAPPLICATION == YES
    /* OS application id            */  , 
#endif
  },
  /* action of the alarm  */  (tpl_action *)&Alarm1_action
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_time_obj, OS_VAR) Alarm1_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&Alarm1_static,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  50,
    /* date                         */  50,
    /* State of the alarm           */  ALARM_AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2VAR(tpl_time_obj, AUTOMATIC, OS_APPL_DATA)
  tpl_alarm_table[ALARM_COUNT] = {
  &Alarm1_alarm_desc
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Declaration of flags functions
 */
/* $FLAGSFUNCTIONS$ */

/*=============================================================================
 * Definition and initialization of Ready List structures
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_heap_entry, OS_VAR) tpl_ready_list[8];
VAR(tpl_rank_count, OS_VAR) tpl_tail_for_prio[7] = {
  0,
  0,
  0,
  0,
  0,
  0
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/**
 * @internal
 *
 * tpl_kern gather informations on the current executing object and
 * the previous one
 */
VAR(tpl_kern_state, OS_VAR) tpl_kern =
{
  NULL,                      /* no running task static descriptor   */
  &IDLE_TASK_task_stat_desc, /* elected task to run is idle task    */
  NULL,                      /* no running task dynamic descriptor  */
  &IDLE_TASK_task_desc,      /* elected task to run is idle task    */
  INVALID_PROC_ID,           /* no running task so no ID            */
  INVALID_PROC_ID,           /* idle task has no ID                 */
  NO_NEED_SWITCH,            /* no context switch needed at start   */
  FALSE,                     /* no schedule needed at start         */
#if WITH_MEMORY_PROTECTION == YES
  1,                         /* at early system startup, we run in  */
                             /*  kernel mode, so in trusted mode    */
#endif /* WITH_MEMORY_PROTECTION */
};

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2CONST(char, AUTOMATIC, OS_APPL_DATA) proc_name_table[TASK_COUNT + ISR_COUNT + 1] = {

  "task1",
  "task2",
  "task3",
  "isr_button2",
  "isr_button1",
  "*idle*"
};
#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/* End of file tpl_app_config.c */