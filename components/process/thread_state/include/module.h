#ifndef CUSTOMOS_PROCESS_THREAD_STATE_H
#define CUSTOMOS_PROCESS_THREAD_STATE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_thread_state_init(void);
unsigned long customos_thread_state_capabilities(void);
const char *customos_thread_state_name(void);
int customos_thread_state_self_check(void);

#endif
