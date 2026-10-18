#include "module.h"

static int initialized;

int customos_priority_policy_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_priority_policy_capabilities(void) {
    return 34UL;
}

const char *customos_priority_policy_name(void) {
    return "priority_policy";
}

int customos_priority_policy_self_check(void) {
    return initialized == 1 && customos_priority_policy_capabilities() == 34UL;
}
