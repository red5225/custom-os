#ifndef CUSTOMOS_PROCESS_TASK_METRICS_H
#define CUSTOMOS_PROCESS_TASK_METRICS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_task_metrics_init(void);
unsigned long customos_task_metrics_capabilities(void);
const char *customos_task_metrics_name(void);
int customos_task_metrics_self_check(void);

#endif
