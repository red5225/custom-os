#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_display_device_name(), "display_device") == 0);
    assert(customos_display_device_capabilities() == 55UL);
    assert(customos_display_device_self_check() == 0);
    assert(customos_display_device_init() == 0);
    assert(customos_display_device_self_check() == 1);
    return 0;
}
