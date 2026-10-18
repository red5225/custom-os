#ifndef CUSTOMOS_UI_UI_FOCUS_H
#define CUSTOMOS_UI_UI_FOCUS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_ui_focus_init(void);
unsigned long customos_ui_focus_capabilities(void);
const char *customos_ui_focus_name(void);
int customos_ui_focus_self_check(void);

#endif
