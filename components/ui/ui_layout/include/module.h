#ifndef CUSTOMOS_UI_UI_LAYOUT_H
#define CUSTOMOS_UI_UI_LAYOUT_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_ui_layout_init(void);
unsigned long customos_ui_layout_capabilities(void);
const char *customos_ui_layout_name(void);
int customos_ui_layout_self_check(void);

#endif
