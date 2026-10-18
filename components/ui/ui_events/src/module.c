#include "module.h"

static int initialized;

int customos_ui_events_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_ui_events_capabilities(void) {
    return 85UL;
}

const char *customos_ui_events_name(void) {
    return "ui_events";
}

int customos_ui_events_self_check(void) {
    return initialized == 1 && customos_ui_events_capabilities() == 85UL;
}
