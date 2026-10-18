#include "module.h"

static int initialized;

int customos_filesystem_check_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_filesystem_check_capabilities(void) {
    return 50UL;
}

const char *customos_filesystem_check_name(void) {
    return "filesystem_check";
}

int customos_filesystem_check_self_check(void) {
    return initialized == 1 && customos_filesystem_check_capabilities() == 50UL;
}
