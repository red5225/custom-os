#include "module.h"

static int initialized;

int customos_physical_pages_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_physical_pages_capabilities(void) {
    return 21UL;
}

const char *customos_physical_pages_name(void) {
    return "physical_pages";
}

int customos_physical_pages_self_check(void) {
    return initialized == 1 && customos_physical_pages_capabilities() == 21UL;
}
