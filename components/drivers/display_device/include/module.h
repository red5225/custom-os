#ifndef CUSTOMOS_DRIVERS_DISPLAY_DEVICE_H
#define CUSTOMOS_DRIVERS_DISPLAY_DEVICE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_display_device_init(void);
unsigned long customos_display_device_capabilities(void);
const char *customos_display_device_name(void);
int customos_display_device_self_check(void);

#endif
