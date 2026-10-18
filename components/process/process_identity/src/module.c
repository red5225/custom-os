#include "module.h"

static int initialized;

int customos_process_identity_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_process_identity_capabilities(void) {
    return 39UL;
}

const char *customos_process_identity_name(void) {
    return "process_identity";
}

int customos_process_identity_self_check(void) {
    return initialized == 1 && customos_process_identity_capabilities() == 39UL;
}
