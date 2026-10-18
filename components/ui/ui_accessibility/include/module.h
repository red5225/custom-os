#ifndef CUSTOMOS_UI_UI_ACCESSIBILITY_H
#define CUSTOMOS_UI_UI_ACCESSIBILITY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_ui_accessibility_init(void);
unsigned long customos_ui_accessibility_capabilities(void);
const char *customos_ui_accessibility_name(void);
int customos_ui_accessibility_self_check(void);

#endif
