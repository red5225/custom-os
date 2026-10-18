#ifndef CUSTOMOS_STORAGE_BLOCK_DEVICE_H
#define CUSTOMOS_STORAGE_BLOCK_DEVICE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_block_device_init(void);
unsigned long customos_block_device_capabilities(void);
const char *customos_block_device_name(void);
int customos_block_device_self_check(void);

#endif
