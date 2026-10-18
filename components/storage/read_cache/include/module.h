#ifndef CUSTOMOS_STORAGE_READ_CACHE_H
#define CUSTOMOS_STORAGE_READ_CACHE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_read_cache_init(void);
unsigned long customos_read_cache_capabilities(void);
const char *customos_read_cache_name(void);
int customos_read_cache_self_check(void);

#endif
