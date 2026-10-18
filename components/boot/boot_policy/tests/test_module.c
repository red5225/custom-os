#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_boot_policy_name(), "boot_policy") == 0);
    assert(customos_boot_policy_capabilities() == 5UL);
    assert(customos_boot_policy_self_check() == 0);
    assert(customos_boot_policy_init() == 0);
    assert(customos_boot_policy_self_check() == 1);
    return 0;
}
