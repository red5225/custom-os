#ifndef CUSTOMOS_PROCESS_TASK_SCHEDULER_H
#define CUSTOMOS_PROCESS_TASK_SCHEDULER_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_task_scheduler_init(void);
unsigned long customos_task_scheduler_capabilities(void);
const char *customos_task_scheduler_name(void);
int customos_task_scheduler_self_check(void);

#endif
