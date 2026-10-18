#include "module.h"

static int initialized;

int customos_integrity_manifest_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_integrity_manifest_capabilities(void) {
    return 91UL;
}

const char *customos_integrity_manifest_name(void) {
    return "integrity_manifest";
}

int customos_integrity_manifest_self_check(void) {
    return initialized == 1 && customos_integrity_manifest_capabilities() == 91UL;
}
