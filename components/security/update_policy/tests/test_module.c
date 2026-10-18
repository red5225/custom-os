#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_update_policy_name(), "update_policy") == 0);
    assert(customos_update_policy_capabilities() == 98UL);
    assert(customos_update_policy_self_check() == 0);
    assert(customos_update_policy_init() == 0);
    assert(customos_update_policy_self_check() == 1);
    return 0;
}
