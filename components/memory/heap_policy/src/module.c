#include "module.h"

static int initialized;

int customos_heap_policy_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_heap_policy_capabilities(void) {
    return 23UL;
}

const char *customos_heap_policy_name(void) {
    return "heap_policy";
}

int customos_heap_policy_self_check(void) {
    return initialized == 1 && customos_heap_policy_capabilities() == 23UL;
}
