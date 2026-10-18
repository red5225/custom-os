#ifndef CUSTOMOS_PROCESS_SIGNAL_ROUTER_H
#define CUSTOMOS_PROCESS_SIGNAL_ROUTER_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_signal_router_init(void);
unsigned long customos_signal_router_capabilities(void);
const char *customos_signal_router_name(void);
int customos_signal_router_self_check(void);

#endif
