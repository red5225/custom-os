#ifndef CUSTOMOS_TERMINAL_INPUT_DISPATCH_H
#define CUSTOMOS_TERMINAL_INPUT_DISPATCH_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_input_dispatch_init(void);
unsigned long customos_input_dispatch_capabilities(void);
const char *customos_input_dispatch_name(void);
int customos_input_dispatch_self_check(void);

#endif
