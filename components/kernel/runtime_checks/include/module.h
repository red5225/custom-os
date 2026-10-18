#ifndef CUSTOMOS_KERNEL_RUNTIME_CHECKS_H
#define CUSTOMOS_KERNEL_RUNTIME_CHECKS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_runtime_checks_init(void);
unsigned long customos_runtime_checks_capabilities(void);
const char *customos_runtime_checks_name(void);
int customos_runtime_checks_self_check(void);

#endif
