#ifndef CUSTOMOS_PROCESS_PROCESS_REGISTRY_H
#define CUSTOMOS_PROCESS_PROCESS_REGISTRY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_process_registry_init(void);
unsigned long customos_process_registry_capabilities(void);
const char *customos_process_registry_name(void);
int customos_process_registry_self_check(void);

#endif
