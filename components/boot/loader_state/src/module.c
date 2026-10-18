#include "module.h"

static int initialized;

int customos_loader_state_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_loader_state_capabilities(void) {
    return 6UL;
}

const char *customos_loader_state_name(void) {
    return "loader_state";
}

int customos_loader_state_self_check(void) {
    return initialized == 1 && customos_loader_state_capabilities() == 6UL;
}
