#include "module.h"

static int initialized;

int customos_input_dispatch_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_input_dispatch_capabilities(void) {
    return 68UL;
}

const char *customos_input_dispatch_name(void) {
    return "input_dispatch";
}

int customos_input_dispatch_self_check(void) {
    return initialized == 1 && customos_input_dispatch_capabilities() == 68UL;
}
