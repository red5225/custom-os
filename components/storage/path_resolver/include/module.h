#ifndef CUSTOMOS_STORAGE_PATH_RESOLVER_H
#define CUSTOMOS_STORAGE_PATH_RESOLVER_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_path_resolver_init(void);
unsigned long customos_path_resolver_capabilities(void);
const char *customos_path_resolver_name(void);
int customos_path_resolver_self_check(void);

#endif
