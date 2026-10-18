#ifndef CUSTOMOS_MEMORY_MEMORY_MAP_H
#define CUSTOMOS_MEMORY_MEMORY_MAP_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_memory_map_init(void);
unsigned long customos_memory_map_capabilities(void);
const char *customos_memory_map_name(void);
int customos_memory_map_self_check(void);

#endif
