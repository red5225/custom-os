#ifndef CUSTOMOS_KERNEL_CPU_FEATURES_H
#define CUSTOMOS_KERNEL_CPU_FEATURES_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_cpu_features_init(void);
unsigned long customos_cpu_features_capabilities(void);
const char *customos_cpu_features_name(void);
int customos_cpu_features_self_check(void);

#endif
