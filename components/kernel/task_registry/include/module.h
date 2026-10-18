#ifndef CUSTOMOS_KERNEL_TASK_REGISTRY_H
#define CUSTOMOS_KERNEL_TASK_REGISTRY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_task_registry_init(void);
unsigned long customos_task_registry_capabilities(void);
const char *customos_task_registry_name(void);
int customos_task_registry_self_check(void);

#endif
