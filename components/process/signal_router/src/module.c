#include "module.h"

static int initialized;

int customos_signal_router_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_signal_router_capabilities(void) {
    return 36UL;
}

const char *customos_signal_router_name(void) {
    return "signal_router";
}

int customos_signal_router_self_check(void) {
    return initialized == 1 && customos_signal_router_capabilities() == 36UL;
}
