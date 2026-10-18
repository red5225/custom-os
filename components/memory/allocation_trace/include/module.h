#ifndef CUSTOMOS_MEMORY_ALLOCATION_TRACE_H
#define CUSTOMOS_MEMORY_ALLOCATION_TRACE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_allocation_trace_init(void);
unsigned long customos_allocation_trace_capabilities(void);
const char *customos_allocation_trace_name(void);
int customos_allocation_trace_self_check(void);

#endif
