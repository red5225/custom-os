#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_keyboard_device_name(), "keyboard_device") == 0);
    assert(customos_keyboard_device_capabilities() == 53UL);
    assert(customos_keyboard_device_self_check() == 0);
    assert(customos_keyboard_device_init() == 0);
    assert(customos_keyboard_device_self_check() == 1);
    return 0;
}
