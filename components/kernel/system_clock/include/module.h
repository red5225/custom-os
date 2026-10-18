#ifndef CUSTOMOS_KERNEL_SYSTEM_CLOCK_H
#define CUSTOMOS_KERNEL_SYSTEM_CLOCK_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_system_clock_init(void);
unsigned long customos_system_clock_capabilities(void);
const char *customos_system_clock_name(void);
int customos_system_clock_self_check(void);

#endif
