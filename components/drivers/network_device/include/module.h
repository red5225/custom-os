#ifndef CUSTOMOS_DRIVERS_NETWORK_DEVICE_H
#define CUSTOMOS_DRIVERS_NETWORK_DEVICE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_network_device_init(void);
unsigned long customos_network_device_capabilities(void);
const char *customos_network_device_name(void);
int customos_network_device_self_check(void);

#endif
