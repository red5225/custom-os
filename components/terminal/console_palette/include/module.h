#ifndef CUSTOMOS_TERMINAL_CONSOLE_PALETTE_H
#define CUSTOMOS_TERMINAL_CONSOLE_PALETTE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_console_palette_init(void);
unsigned long customos_console_palette_capabilities(void);
const char *customos_console_palette_name(void);
int customos_console_palette_self_check(void);

#endif
