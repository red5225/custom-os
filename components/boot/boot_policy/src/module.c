#include "module.h"

static int initialized;

int customos_boot_policy_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_boot_policy_capabilities(void) {
    return 5UL;
}

const char *customos_boot_policy_name(void) {
    return "boot_policy";
}

int customos_boot_policy_self_check(void) {
    return initialized == 1 && customos_boot_policy_capabilities() == 5UL;
}
