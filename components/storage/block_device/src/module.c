#include "module.h"

static int initialized;

int customos_block_device_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_block_device_capabilities(void) {
    return 41UL;
}

const char *customos_block_device_name(void) {
    return "block_device";
}

int customos_block_device_self_check(void) {
    return initialized == 1 && customos_block_device_capabilities() == 41UL;
}
