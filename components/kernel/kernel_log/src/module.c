#include "module.h"

static int initialized;

int customos_kernel_log_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_kernel_log_capabilities(void) {
    return 18UL;
}

const char *customos_kernel_log_name(void) {
    return "kernel_log";
}

int customos_kernel_log_self_check(void) {
    return initialized == 1 && customos_kernel_log_capabilities() == 18UL;
}
