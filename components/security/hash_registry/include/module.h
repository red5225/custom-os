#ifndef CUSTOMOS_SECURITY_HASH_REGISTRY_H
#define CUSTOMOS_SECURITY_HASH_REGISTRY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_hash_registry_init(void);
unsigned long customos_hash_registry_capabilities(void);
const char *customos_hash_registry_name(void);
int customos_hash_registry_self_check(void);

#endif
