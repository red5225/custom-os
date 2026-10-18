#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_task_registry_name(), "task_registry") == 0);
    assert(customos_task_registry_capabilities() == 17UL);
    assert(customos_task_registry_self_check() == 0);
    assert(customos_task_registry_init() == 0);
    assert(customos_task_registry_self_check() == 1);
    return 0;
}
