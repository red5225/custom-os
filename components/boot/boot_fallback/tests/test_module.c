#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_boot_fallback_name(), "boot_fallback") == 0);
    assert(customos_boot_fallback_capabilities() == 8UL);
    assert(customos_boot_fallback_self_check() == 0);
    assert(customos_boot_fallback_init() == 0);
    assert(customos_boot_fallback_self_check() == 1);
    return 0;
}
