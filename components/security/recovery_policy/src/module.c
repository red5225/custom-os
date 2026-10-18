#include "module.h"

static int initialized;

int customos_recovery_policy_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_recovery_policy_capabilities(void) {
    return 100UL;
}

const char *customos_recovery_policy_name(void) {
    return "recovery_policy";
}

int customos_recovery_policy_self_check(void) {
    return initialized == 1 && customos_recovery_policy_capabilities() == 100UL;
}
