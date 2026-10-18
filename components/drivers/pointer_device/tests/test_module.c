#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_pointer_device_name(), "pointer_device") == 0);
    assert(customos_pointer_device_capabilities() == 54UL);
    assert(customos_pointer_device_self_check() == 0);
    assert(customos_pointer_device_init() == 0);
    assert(customos_pointer_device_self_check() == 1);
    return 0;
}
