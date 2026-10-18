#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_boot_menu_name(), "boot_menu") == 0);
    assert(customos_boot_menu_capabilities() == 3UL);
    assert(customos_boot_menu_self_check() == 0);
    assert(customos_boot_menu_init() == 0);
    assert(customos_boot_menu_self_check() == 1);
    return 0;
}
