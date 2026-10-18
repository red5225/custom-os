#ifndef CUSTOMOS_PROCESS_PROCESS_LIMITS_H
#define CUSTOMOS_PROCESS_PROCESS_LIMITS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_process_limits_init(void);
unsigned long customos_process_limits_capabilities(void);
const char *customos_process_limits_name(void);
int customos_process_limits_self_check(void);

#endif
