#include "module.h"

static int initialized;

int customos_task_scheduler_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_task_scheduler_capabilities(void) {
    return 32UL;
}

const char *customos_task_scheduler_name(void) {
    return "task_scheduler";
}

int customos_task_scheduler_self_check(void) {
    return initialized == 1 && customos_task_scheduler_capabilities() == 32UL;
}
