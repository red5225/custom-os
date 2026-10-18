#ifndef CUSTOMOS_PROCESS_WAIT_QUEUE_H
#define CUSTOMOS_PROCESS_WAIT_QUEUE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_wait_queue_init(void);
unsigned long customos_wait_queue_capabilities(void);
const char *customos_wait_queue_name(void);
int customos_wait_queue_self_check(void);

#endif
