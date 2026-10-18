#ifndef CUSTOMOS_DRIVERS_USB_REGISTRY_H
#define CUSTOMOS_DRIVERS_USB_REGISTRY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_usb_registry_init(void);
unsigned long customos_usb_registry_capabilities(void);
const char *customos_usb_registry_name(void);
int customos_usb_registry_self_check(void);

#endif
