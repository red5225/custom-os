#ifndef CUSTOMOS_UI_UI_NOTIFICATIONS_H
#define CUSTOMOS_UI_UI_NOTIFICATIONS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_ui_notifications_init(void);
unsigned long customos_ui_notifications_capabilities(void);
const char *customos_ui_notifications_name(void);
int customos_ui_notifications_self_check(void);

#endif
