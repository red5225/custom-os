#ifndef CUSTOMOS_UI_UI_EVENTS_H
#define CUSTOMOS_UI_UI_EVENTS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_ui_events_init(void);
unsigned long customos_ui_events_capabilities(void);
const char *customos_ui_events_name(void);
int customos_ui_events_self_check(void);

#endif
