#include "module.h"

static int initialized;

int customos_firmware_probe_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_firmware_probe_capabilities(void) {
    return 9UL;
}

const char *customos_firmware_probe_name(void) {
    return "firmware_probe";
}

int customos_firmware_probe_self_check(void) {
    return initialized == 1 && customos_firmware_probe_capabilities() == 9UL;
}
