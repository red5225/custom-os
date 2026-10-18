#ifndef CUSTOMOS_TERMINAL_LINE_EDITOR_H
#define CUSTOMOS_TERMINAL_LINE_EDITOR_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_line_editor_init(void);
unsigned long customos_line_editor_capabilities(void);
const char *customos_line_editor_name(void);
int customos_line_editor_self_check(void);

#endif
