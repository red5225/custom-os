#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_permission_policy_name(), "permission_policy") == 0);
    assert(customos_permission_policy_capabilities() == 93UL);
    assert(customos_permission_policy_self_check() == 0);
    assert(customos_permission_policy_init() == 0);
    assert(customos_permission_policy_self_check() == 1);
    return 0;
}
