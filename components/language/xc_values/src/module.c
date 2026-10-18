#include "module.h"

static int initialized;

int customos_xc_values_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_xc_values_capabilities(void) {
    return 73UL;
}

const char *customos_xc_values_name(void) {
    return "xc_values";
}

int customos_xc_values_self_check(void) {
    return initialized == 1 && customos_xc_values_capabilities() == 73UL;
}
