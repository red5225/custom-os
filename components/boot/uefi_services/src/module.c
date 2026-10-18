#include "module.h"

static int initialized;

int customos_uefi_services_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_uefi_services_capabilities(void) {
    return 2UL;
}

const char *customos_uefi_services_name(void) {
    return "uefi_services";
}

int customos_uefi_services_self_check(void) {
    return initialized == 1 && customos_uefi_services_capabilities() == 2UL;
}
