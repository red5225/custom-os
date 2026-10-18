#ifndef CUSTOMOS_MEMORY_HEAP_POLICY_H
#define CUSTOMOS_MEMORY_HEAP_POLICY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_heap_policy_init(void);
unsigned long customos_heap_policy_capabilities(void);
const char *customos_heap_policy_name(void);
int customos_heap_policy_self_check(void);

#endif
