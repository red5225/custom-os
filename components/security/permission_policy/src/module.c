#include "module.h"

static int initialized;

int customos_permission_policy_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_permission_policy_capabilities(void) {
    return 93UL;
}

const char *customos_permission_policy_name(void) {
    return "permission_policy";
}

int customos_permission_policy_self_check(void) {
    return initialized == 1 && customos_permission_policy_capabilities() == 93UL;
}
