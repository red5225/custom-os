#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_kernel_log_name(), "kernel_log") == 0);
    assert(customos_kernel_log_capabilities() == 18UL);
    assert(customos_kernel_log_self_check() == 0);
    assert(customos_kernel_log_init() == 0);
    assert(customos_kernel_log_self_check() == 1);
    return 0;
}
