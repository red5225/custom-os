#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_kernel_version_name(), "kernel_version") == 0);
    assert(customos_kernel_version_capabilities() == 20UL);
    assert(customos_kernel_version_self_check() == 0);
    assert(customos_kernel_version_init() == 0);
    assert(customos_kernel_version_self_check() == 1);
    return 0;
}
