#include "module.h"

static int initialized;

int customos_interrupt_router_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_interrupt_router_capabilities(void) {
    return 15UL;
}

const char *customos_interrupt_router_name(void) {
    return "interrupt_router";
}

int customos_interrupt_router_self_check(void) {
    return initialized == 1 && customos_interrupt_router_capabilities() == 15UL;
}
