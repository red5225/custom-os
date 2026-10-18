#ifndef CUSTOMOS_DRIVERS_DRIVER_REGISTRY_H
#define CUSTOMOS_DRIVERS_DRIVER_REGISTRY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_driver_registry_init(void);
unsigned long customos_driver_registry_capabilities(void);
const char *customos_driver_registry_name(void);
int customos_driver_registry_self_check(void);

#endif
