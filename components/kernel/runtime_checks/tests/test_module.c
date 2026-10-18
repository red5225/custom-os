#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_runtime_checks_name(), "runtime_checks") == 0);
    assert(customos_runtime_checks_capabilities() == 19UL);
    assert(customos_runtime_checks_self_check() == 0);
    assert(customos_runtime_checks_init() == 0);
    assert(customos_runtime_checks_self_check() == 1);
    return 0;
}
