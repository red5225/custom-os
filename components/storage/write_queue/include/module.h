#ifndef CUSTOMOS_STORAGE_WRITE_QUEUE_H
#define CUSTOMOS_STORAGE_WRITE_QUEUE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_write_queue_init(void);
unsigned long customos_write_queue_capabilities(void);
const char *customos_write_queue_name(void);
int customos_write_queue_self_check(void);

#endif
