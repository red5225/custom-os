#ifndef CUSTOMOS_UI_UI_LAUNCHER_H
#define CUSTOMOS_UI_UI_LAUNCHER_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_ui_launcher_init(void);
unsigned long customos_ui_launcher_capabilities(void);
const char *customos_ui_launcher_name(void);
int customos_ui_launcher_self_check(void);

#endif
