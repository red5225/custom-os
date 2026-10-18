#include "module.h"

static int initialized;

int customos_display_device_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_display_device_capabilities(void) {
    return 55UL;
}

const char *customos_display_device_name(void) {
    return "display_device";
}

int customos_display_device_self_check(void) {
    return initialized == 1 && customos_display_device_capabilities() == 55UL;
}
