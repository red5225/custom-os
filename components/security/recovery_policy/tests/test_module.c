#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_recovery_policy_name(), "recovery_policy") == 0);
    assert(customos_recovery_policy_capabilities() == 100UL);
    assert(customos_recovery_policy_self_check() == 0);
    assert(customos_recovery_policy_init() == 0);
    assert(customos_recovery_policy_self_check() == 1);
    return 0;
}
