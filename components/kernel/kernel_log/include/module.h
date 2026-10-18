#ifndef CUSTOMOS_KERNEL_KERNEL_LOG_H
#define CUSTOMOS_KERNEL_KERNEL_LOG_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_kernel_log_init(void);
unsigned long customos_kernel_log_capabilities(void);
const char *customos_kernel_log_name(void);
int customos_kernel_log_self_check(void);

#endif
