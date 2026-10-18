#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_shell_policy_name(), "shell_policy") == 0);
    assert(customos_shell_policy_capabilities() == 70UL);
    assert(customos_shell_policy_self_check() == 0);
    assert(customos_shell_policy_init() == 0);
    assert(customos_shell_policy_self_check() == 1);
    return 0;
}
