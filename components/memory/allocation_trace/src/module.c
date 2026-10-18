#include "module.h"

static int initialized;

int customos_allocation_trace_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_allocation_trace_capabilities(void) {
    return 26UL;
}

const char *customos_allocation_trace_name(void) {
    return "allocation_trace";
}

int customos_allocation_trace_self_check(void) {
    return initialized == 1 && customos_allocation_trace_capabilities() == 26UL;
}
