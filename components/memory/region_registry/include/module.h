#ifndef CUSTOMOS_MEMORY_REGION_REGISTRY_H
#define CUSTOMOS_MEMORY_REGION_REGISTRY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_region_registry_init(void);
unsigned long customos_region_registry_capabilities(void);
const char *customos_region_registry_name(void);
int customos_region_registry_self_check(void);

#endif
