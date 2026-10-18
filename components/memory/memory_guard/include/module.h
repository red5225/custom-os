#ifndef CUSTOMOS_MEMORY_MEMORY_GUARD_H
#define CUSTOMOS_MEMORY_MEMORY_GUARD_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_memory_guard_init(void);
unsigned long customos_memory_guard_capabilities(void);
const char *customos_memory_guard_name(void);
int customos_memory_guard_self_check(void);

#endif
