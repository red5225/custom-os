#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_usb_registry_name(), "usb_registry") == 0);
    assert(customos_usb_registry_capabilities() == 52UL);
    assert(customos_usb_registry_self_check() == 0);
    assert(customos_usb_registry_init() == 0);
    assert(customos_usb_registry_self_check() == 1);
    return 0;
}
