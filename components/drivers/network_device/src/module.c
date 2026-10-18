#include "module.h"

static int initialized;

int customos_network_device_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_network_device_capabilities(void) {
    return 57UL;
}

const char *customos_network_device_name(void) {
    return "network_device";
}

int customos_network_device_self_check(void) {
    return initialized == 1 && customos_network_device_capabilities() == 57UL;
}
