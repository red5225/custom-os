#include "module.h"

static int initialized;

int customos_ui_accessibility_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_ui_accessibility_capabilities(void) {
    return 86UL;
}

const char *customos_ui_accessibility_name(void) {
    return "ui_accessibility";
}

int customos_ui_accessibility_self_check(void) {
    return initialized == 1 && customos_ui_accessibility_capabilities() == 86UL;
}
