#include "module.h"

static int initialized;

int customos_xc_functions_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_xc_functions_capabilities(void) {
    return 77UL;
}

const char *customos_xc_functions_name(void) {
    return "xc_functions";
}

int customos_xc_functions_self_check(void) {
    return initialized == 1 && customos_xc_functions_capabilities() == 77UL;
}
