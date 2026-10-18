#include "module.h"

static int initialized;

int customos_xc_parser_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_xc_parser_capabilities(void) {
    return 72UL;
}

const char *customos_xc_parser_name(void) {
    return "xc_parser";
}

int customos_xc_parser_self_check(void) {
    return initialized == 1 && customos_xc_parser_capabilities() == 72UL;
}
