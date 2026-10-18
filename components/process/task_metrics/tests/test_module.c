#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_task_metrics_name(), "task_metrics") == 0);
    assert(customos_task_metrics_capabilities() == 38UL);
    assert(customos_task_metrics_self_check() == 0);
    assert(customos_task_metrics_init() == 0);
    assert(customos_task_metrics_self_check() == 1);
    return 0;
}
