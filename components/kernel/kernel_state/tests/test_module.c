#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_kernel_state_name(), "kernel_state") == 0);
    assert(customos_kernel_state_capabilities() == 11UL);
    assert(customos_kernel_state_self_check() == 0);
    assert(customos_kernel_state_init() == 0);
    assert(customos_kernel_state_self_check() == 1);
    return 0;
}
