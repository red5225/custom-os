#include "module.h"

static int initialized;

int customos_keyboard_device_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_keyboard_device_capabilities(void) {
    return 53UL;
}

const char *customos_keyboard_device_name(void) {
    return "keyboard_device";
}

int customos_keyboard_device_self_check(void) {
    return initialized == 1 && customos_keyboard_device_capabilities() == 53UL;
}
