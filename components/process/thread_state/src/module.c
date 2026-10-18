#include "module.h"

static int initialized;

int customos_thread_state_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_thread_state_capabilities(void) {
    return 33UL;
}

const char *customos_thread_state_name(void) {
    return "thread_state";
}

int customos_thread_state_self_check(void) {
    return initialized == 1 && customos_thread_state_capabilities() == 33UL;
}
