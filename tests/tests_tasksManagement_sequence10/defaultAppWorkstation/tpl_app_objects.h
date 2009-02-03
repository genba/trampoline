/*
 * file: $HEADER$
 *
 * This file contains various defines used by Trampoline
 * for conditional compiling.
 *
 * This file has been generated automatically by goil
 * on  Tue Jan 20 16:31:17 2009
 */

#ifndef OJB_HEADER_H
#define OJB_HEADER_H

/*
 * Configuration flags
 */
#define    WITH_STARTUP_HOOK
#define    WITH_SHUTDOWN_HOOK
#define    WITH_ERROR_HOOK
#define    WITH_PRE_TASK_HOOK


/*
 * Count of the objects used in the application.
 * These informations are used by Trampoline to avoid to
 * alloc structures when some os objects are not present.
 *
 */

/*
 * Declaration of the priority levels count
 */
#define PRIO_LEVEL_COUNT        3

/*
 * Declaration of Task object count
 */
#define TASK_COUNT              3

/*
 * Declaration of Extended task count
 */
#define EXTENDED_TASK_COUNT     2

/*
 * Declaration of ISR2 object count
 */
#define ISR_COUNT             0

/*
 * Declaration of Alarm object count
 */
#define ALARM_COUNT           0

/*
 * Declaration of Resource object count
 */
#define RESOURCE_COUNT        1

/*
 * Declaration of Send Message object count
 */
#define SEND_MESSAGE_COUNT    0

/*
 * Declaration of Receive Message object count
 */
#define RECEIVE_MESSAGE_COUNT 0

#ifdef WITH_AUTOSAR
/*
 * Declaration of Schedule Table object count
 */
#define SCHEDTABLE_COUNT      $SCHEDTABLE_COUNT$

/*
 * Declaration of Software Counters object count
 */
#define COUNTER_COUNT         0

/*
 * Declaration of OS Application object count
 */
#define APP_COUNT             $APP_COUNT$

/*
 * Declaration of the number of virtual watchdogs used for timing protection
 */
#define MAXIMUM_SCHEDULED_WATCHDOGS $MAX_WATCH_DOG$
#endif

/*
 * Define of RES_SCHEDULER priority
 */
#define RES_SCHEDULER_PRIORITY  2

#endif

/* End of file $HEADER$ */
