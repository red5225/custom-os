#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_boot_logging_name(), "boot_logging") == 0);
    assert(customos_boot_logging_capabilities() == 7UL);
    assert(customos_boot_logging_self_check() == 0);
    assert(customos_boot_logging_init() == 0);
    assert(customos_boot_logging_self_check() == 1);
    return 0;
}
