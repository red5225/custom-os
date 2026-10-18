#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_keymap_registry_name(), "keymap_registry") == 0);
    assert(customos_keymap_registry_capabilities() == 63UL);
    assert(customos_keymap_registry_self_check() == 0);
    assert(customos_keymap_registry_init() == 0);
    assert(customos_keymap_registry_self_check() == 1);
    return 0;
}
