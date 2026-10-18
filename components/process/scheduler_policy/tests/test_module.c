#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_scheduler_policy_name(), "scheduler_policy") == 0);
    assert(customos_scheduler_policy_capabilities() == 40UL);
    assert(customos_scheduler_policy_self_check() == 0);
    assert(customos_scheduler_policy_init() == 0);
    assert(customos_scheduler_policy_self_check() == 1);
    return 0;
}
