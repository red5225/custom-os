#include "module.h"

static int initialized;

int customos_console_palette_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_console_palette_capabilities(void) {
    return 66UL;
}

const char *customos_console_palette_name(void) {
    return "console_palette";
}

int customos_console_palette_self_check(void) {
    return initialized == 1 && customos_console_palette_capabilities() == 66UL;
}
