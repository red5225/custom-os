#include "module.h"

static int initialized;

int customos_update_policy_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_update_policy_capabilities(void) {
    return 98UL;
}

const char *customos_update_policy_name(void) {
    return "update_policy";
}

int customos_update_policy_self_check(void) {
    return initialized == 1 && customos_update_policy_capabilities() == 98UL;
}
