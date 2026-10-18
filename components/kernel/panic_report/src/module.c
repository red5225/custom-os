#include "module.h"

static int initialized;

int customos_panic_report_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_panic_report_capabilities(void) {
    return 12UL;
}

const char *customos_panic_report_name(void) {
    return "panic_report";
}

int customos_panic_report_self_check(void) {
    return initialized == 1 && customos_panic_report_capabilities() == 12UL;
}
