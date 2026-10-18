#include "module.h"

static int initialized;

int customos_terminal_buffer_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_terminal_buffer_capabilities(void) {
    return 61UL;
}

const char *customos_terminal_buffer_name(void) {
    return "terminal_buffer";
}

int customos_terminal_buffer_self_check(void) {
    return initialized == 1 && customos_terminal_buffer_capabilities() == 61UL;
}
