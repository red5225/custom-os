#ifndef CUSTOMOS_UI_UI_SETTINGS_H
#define CUSTOMOS_UI_UI_SETTINGS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_ui_settings_init(void);
unsigned long customos_ui_settings_capabilities(void);
const char *customos_ui_settings_name(void);
int customos_ui_settings_self_check(void);

#endif
