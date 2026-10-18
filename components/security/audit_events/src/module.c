#include "module.h"

static int initialized;

int customos_audit_events_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_audit_events_capabilities(void) {
    return 95UL;
}

const char *customos_audit_events_name(void) {
    return "audit_events";
}

int customos_audit_events_self_check(void) {
    return initialized == 1 && customos_audit_events_capabilities() == 95UL;
}
