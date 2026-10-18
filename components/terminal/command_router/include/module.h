#ifndef CUSTOMOS_TERMINAL_COMMAND_ROUTER_H
#define CUSTOMOS_TERMINAL_COMMAND_ROUTER_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_command_router_init(void);
unsigned long customos_command_router_capabilities(void);
const char *customos_command_router_name(void);
int customos_command_router_self_check(void);

#endif
