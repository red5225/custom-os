#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_capability_policy_name(), "capability_policy") == 0);
    assert(customos_capability_policy_capabilities() == 96UL);
    assert(customos_capability_policy_self_check() == 0);
    assert(customos_capability_policy_init() == 0);
    assert(customos_capability_policy_self_check() == 1);
    return 0;
}
