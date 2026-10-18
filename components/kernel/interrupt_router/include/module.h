#ifndef CUSTOMOS_KERNEL_INTERRUPT_ROUTER_H
#define CUSTOMOS_KERNEL_INTERRUPT_ROUTER_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_interrupt_router_init(void);
unsigned long customos_interrupt_router_capabilities(void);
const char *customos_interrupt_router_name(void);
int customos_interrupt_router_self_check(void);

#endif
