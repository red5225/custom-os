#include "module.h"

static int initialized;

int customos_xc_tokens_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_xc_tokens_capabilities(void) {
    return 71UL;
}

const char *customos_xc_tokens_name(void) {
    return "xc_tokens";
}

int customos_xc_tokens_self_check(void) {
    return initialized == 1 && customos_xc_tokens_capabilities() == 71UL;
}
