#ifndef CUSTOMOS_STORAGE_VOLUME_REGISTRY_H
#define CUSTOMOS_STORAGE_VOLUME_REGISTRY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_volume_registry_init(void);
unsigned long customos_volume_registry_capabilities(void);
const char *customos_volume_registry_name(void);
int customos_volume_registry_self_check(void);

#endif
