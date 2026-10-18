#include "module.h"

static int initialized;

int customos_xc_arithmetic_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_xc_arithmetic_capabilities(void) {
    return 75UL;
}

const char *customos_xc_arithmetic_name(void) {
    return "xc_arithmetic";
}

int customos_xc_arithmetic_self_check(void) {
    return initialized == 1 && customos_xc_arithmetic_capabilities() == 75UL;
}
