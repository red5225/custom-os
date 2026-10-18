#ifndef CUSTOMOS_UI_UI_STATUS_PANEL_H
#define CUSTOMOS_UI_UI_STATUS_PANEL_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_ui_status_panel_init(void);
unsigned long customos_ui_status_panel_capabilities(void);
const char *customos_ui_status_panel_name(void);
int customos_ui_status_panel_self_check(void);

#endif
