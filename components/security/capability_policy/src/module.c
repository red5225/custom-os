#include "module.h"

static int initialized;

int customos_capability_policy_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_capability_policy_capabilities(void) {
    return 96UL;
}

const char *customos_capability_policy_name(void) {
    return "capability_policy";
}

int customos_capability_policy_self_check(void) {
    return initialized == 1 && customos_capability_policy_capabilities() == 96UL;
}
