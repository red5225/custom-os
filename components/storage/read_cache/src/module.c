#include "module.h"

static int initialized;

int customos_read_cache_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_read_cache_capabilities(void) {
    return 48UL;
}

const char *customos_read_cache_name(void) {
    return "read_cache";
}

int customos_read_cache_self_check(void) {
    return initialized == 1 && customos_read_cache_capabilities() == 48UL;
}
