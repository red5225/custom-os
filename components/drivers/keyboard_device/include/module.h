#ifndef CUSTOMOS_DRIVERS_KEYBOARD_DEVICE_H
#define CUSTOMOS_DRIVERS_KEYBOARD_DEVICE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_keyboard_device_init(void);
unsigned long customos_keyboard_device_capabilities(void);
const char *customos_keyboard_device_name(void);
int customos_keyboard_device_self_check(void);

#endif
