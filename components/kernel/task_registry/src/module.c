#include "module.h"

static int initialized;

int customos_task_registry_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_task_registry_capabilities(void) {
    return 17UL;
}

const char *customos_task_registry_name(void) {
    return "task_registry";
}

int customos_task_registry_self_check(void) {
    return initialized == 1 && customos_task_registry_capabilities() == 17UL;
}
