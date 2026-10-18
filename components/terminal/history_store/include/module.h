#ifndef CUSTOMOS_TERMINAL_HISTORY_STORE_H
#define CUSTOMOS_TERMINAL_HISTORY_STORE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_history_store_init(void);
unsigned long customos_history_store_capabilities(void);
const char *customos_history_store_name(void);
int customos_history_store_self_check(void);

#endif
