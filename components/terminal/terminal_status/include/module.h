#ifndef CUSTOMOS_TERMINAL_TERMINAL_STATUS_H
#define CUSTOMOS_TERMINAL_TERMINAL_STATUS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_terminal_status_init(void);
unsigned long customos_terminal_status_capabilities(void);
const char *customos_terminal_status_name(void);
int customos_terminal_status_self_check(void);

#endif
