#include "module.h"

static int initialized;

int customos_memory_guard_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_memory_guard_capabilities(void) {
    return 29UL;
}

const char *customos_memory_guard_name(void) {
    return "memory_guard";
}

int customos_memory_guard_self_check(void) {
    return initialized == 1 && customos_memory_guard_capabilities() == 29UL;
}
