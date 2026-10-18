#ifndef CUSTOMOS_TERMINAL_TERMINAL_BUFFER_H
#define CUSTOMOS_TERMINAL_TERMINAL_BUFFER_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_terminal_buffer_init(void);
unsigned long customos_terminal_buffer_capabilities(void);
const char *customos_terminal_buffer_name(void);
int customos_terminal_buffer_self_check(void);

#endif
