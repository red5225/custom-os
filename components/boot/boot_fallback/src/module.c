#include "module.h"

static int initialized;

int customos_boot_fallback_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_boot_fallback_capabilities(void) {
    return 8UL;
}

const char *customos_boot_fallback_name(void) {
    return "boot_fallback";
}

int customos_boot_fallback_self_check(void) {
    return initialized == 1 && customos_boot_fallback_capabilities() == 8UL;
}
