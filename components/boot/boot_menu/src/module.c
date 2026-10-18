#include "module.h"

static int initialized;

int customos_boot_menu_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_boot_menu_capabilities(void) {
    return 3UL;
}

const char *customos_boot_menu_name(void) {
    return "boot_menu";
}

int customos_boot_menu_self_check(void) {
    return initialized == 1 && customos_boot_menu_capabilities() == 3UL;
}
