#include "module.h"

static int initialized;

int customos_cpu_features_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_cpu_features_capabilities(void) {
    return 14UL;
}

const char *customos_cpu_features_name(void) {
    return "cpu_features";
}

int customos_cpu_features_self_check(void) {
    return initialized == 1 && customos_cpu_features_capabilities() == 14UL;
}
