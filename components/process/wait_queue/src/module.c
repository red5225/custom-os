#include "module.h"

static int initialized;

int customos_wait_queue_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_wait_queue_capabilities(void) {
    return 35UL;
}

const char *customos_wait_queue_name(void) {
    return "wait_queue";
}

int customos_wait_queue_self_check(void) {
    return initialized == 1 && customos_wait_queue_capabilities() == 35UL;
}
