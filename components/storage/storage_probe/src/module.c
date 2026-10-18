#include "module.h"

static int initialized;

int customos_storage_probe_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_storage_probe_capabilities(void) {
    return 47UL;
}

const char *customos_storage_probe_name(void) {
    return "storage_probe";
}

int customos_storage_probe_self_check(void) {
    return initialized == 1 && customos_storage_probe_capabilities() == 47UL;
}
