#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_task_scheduler_name(), "task_scheduler") == 0);
    assert(customos_task_scheduler_capabilities() == 32UL);
    assert(customos_task_scheduler_self_check() == 0);
    assert(customos_task_scheduler_init() == 0);
    assert(customos_task_scheduler_self_check() == 1);
    return 0;
}
