#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_xc_runtime_state_name(), "xc_runtime_state") == 0);
    assert(customos_xc_runtime_state_capabilities() == 79UL);
    assert(customos_xc_runtime_state_self_check() == 0);
    assert(customos_xc_runtime_state_init() == 0);
    assert(customos_xc_runtime_state_self_check() == 1);
    return 0;
}
