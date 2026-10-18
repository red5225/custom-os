#include "module.h"

static int initialized;

int customos_scheduler_policy_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_scheduler_policy_capabilities(void) {
    return 40UL;
}

const char *customos_scheduler_policy_name(void) {
    return "scheduler_policy";
}

int customos_scheduler_policy_self_check(void) {
    return initialized == 1 && customos_scheduler_policy_capabilities() == 40UL;
}
