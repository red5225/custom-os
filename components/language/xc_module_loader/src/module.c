#include "module.h"

static int initialized;

int customos_xc_module_loader_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_xc_module_loader_capabilities(void) {
    return 80UL;
}

const char *customos_xc_module_loader_name(void) {
    return "xc_module_loader";
}

int customos_xc_module_loader_self_check(void) {
    return initialized == 1 && customos_xc_module_loader_capabilities() == 80UL;
}
