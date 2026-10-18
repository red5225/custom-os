#ifndef CUSTOMOS_MEMORY_MEMORY_STATS_H
#define CUSTOMOS_MEMORY_MEMORY_STATS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_memory_stats_init(void);
unsigned long customos_memory_stats_capabilities(void);
const char *customos_memory_stats_name(void);
int customos_memory_stats_self_check(void);

#endif
