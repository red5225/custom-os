#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_priority_policy_name(), "priority_policy") == 0);
    assert(customos_priority_policy_capabilities() == 34UL);
    assert(customos_priority_policy_self_check() == 0);
    assert(customos_priority_policy_init() == 0);
    assert(customos_priority_policy_self_check() == 1);
    return 0;
}
