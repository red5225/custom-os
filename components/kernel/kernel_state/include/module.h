#ifndef CUSTOMOS_KERNEL_KERNEL_STATE_H
#define CUSTOMOS_KERNEL_KERNEL_STATE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_kernel_state_init(void);
unsigned long customos_kernel_state_capabilities(void);
const char *customos_kernel_state_name(void);
int customos_kernel_state_self_check(void);

#endif
