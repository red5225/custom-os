#include "module.h"

static int initialized;

int customos_thermal_probe_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_thermal_probe_capabilities(void) {
    return 59UL;
}

const char *customos_thermal_probe_name(void) {
    return "thermal_probe";
}

int customos_thermal_probe_self_check(void) {
    return initialized == 1 && customos_thermal_probe_capabilities() == 59UL;
}
