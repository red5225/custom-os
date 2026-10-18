#include "module.h"

static int initialized;

int customos_memory_stats_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_memory_stats_capabilities(void) {
    return 27UL;
}

const char *customos_memory_stats_name(void) {
    return "memory_stats";
}

int customos_memory_stats_self_check(void) {
    return initialized == 1 && customos_memory_stats_capabilities() == 27UL;
}
