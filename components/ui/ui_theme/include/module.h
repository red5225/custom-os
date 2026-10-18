#ifndef CUSTOMOS_UI_UI_THEME_H
#define CUSTOMOS_UI_UI_THEME_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_ui_theme_init(void);
unsigned long customos_ui_theme_capabilities(void);
const char *customos_ui_theme_name(void);
int customos_ui_theme_self_check(void);

#endif
