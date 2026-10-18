#include "module.h"

static int initialized;

int customos_process_limits_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_process_limits_capabilities(void) {
    return 37UL;
}

const char *customos_process_limits_name(void) {
    return "process_limits";
}

int customos_process_limits_self_check(void) {
    return initialized == 1 && customos_process_limits_capabilities() == 37UL;
}
