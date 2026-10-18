#ifndef CUSTOMOS_KERNEL_KERNEL_VERSION_H
#define CUSTOMOS_KERNEL_KERNEL_VERSION_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_kernel_version_init(void);
unsigned long customos_kernel_version_capabilities(void);
const char *customos_kernel_version_name(void);
int customos_kernel_version_self_check(void);

#endif
