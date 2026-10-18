#ifndef CUSTOMOS_DRIVERS_POINTER_DEVICE_H
#define CUSTOMOS_DRIVERS_POINTER_DEVICE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_pointer_device_init(void);
unsigned long customos_pointer_device_capabilities(void);
const char *customos_pointer_device_name(void);
int customos_pointer_device_self_check(void);

#endif
