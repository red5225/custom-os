#include "module.h"

static int initialized;

int customos_kernel_version_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_kernel_version_capabilities(void) {
    return 20UL;
}

const char *customos_kernel_version_name(void) {
    return "kernel_version";
}

int customos_kernel_version_self_check(void) {
    return initialized == 1 && customos_kernel_version_capabilities() == 20UL;
}
