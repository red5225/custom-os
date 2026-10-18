#include "module.h"

static int initialized;

int customos_line_editor_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_line_editor_capabilities(void) {
    return 62UL;
}

const char *customos_line_editor_name(void) {
    return "line_editor";
}

int customos_line_editor_self_check(void) {
    return initialized == 1 && customos_line_editor_capabilities() == 62UL;
}
