#include "module.h"

static int initialized;

int customos_pointer_device_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_pointer_device_capabilities(void) {
    return 54UL;
}

const char *customos_pointer_device_name(void) {
    return "pointer_device";
}

int customos_pointer_device_self_check(void) {
    return initialized == 1 && customos_pointer_device_capabilities() == 54UL;
}
