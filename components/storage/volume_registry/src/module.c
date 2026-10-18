#include "module.h"

static int initialized;

int customos_volume_registry_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_volume_registry_capabilities(void) {
    return 42UL;
}

const char *customos_volume_registry_name(void) {
    return "volume_registry";
}

int customos_volume_registry_self_check(void) {
    return initialized == 1 && customos_volume_registry_capabilities() == 42UL;
}
