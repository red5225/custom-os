#include "module.h"

static int initialized;

int customos_shell_policy_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_shell_policy_capabilities(void) {
    return 70UL;
}

const char *customos_shell_policy_name(void) {
    return "shell_policy";
}

int customos_shell_policy_self_check(void) {
    return initialized == 1 && customos_shell_policy_capabilities() == 70UL;
}
