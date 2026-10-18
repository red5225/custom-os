#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_boot_config_name(), "boot_config") == 0);
    assert(customos_boot_config_capabilities() == 1UL);
    assert(customos_boot_config_self_check() == 0);
    assert(customos_boot_config_init() == 0);
    assert(customos_boot_config_self_check() == 1);
    return 0;
}
