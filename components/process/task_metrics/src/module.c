#include "module.h"

static int initialized;

int customos_task_metrics_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_task_metrics_capabilities(void) {
    return 38UL;
}

const char *customos_task_metrics_name(void) {
    return "task_metrics";
}

int customos_task_metrics_self_check(void) {
    return initialized == 1 && customos_task_metrics_capabilities() == 38UL;
}
