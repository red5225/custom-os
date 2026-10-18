#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_driver_registry_name(), "driver_registry") == 0);
    assert(customos_driver_registry_capabilities() == 60UL);
    assert(customos_driver_registry_self_check() == 0);
    assert(customos_driver_registry_init() == 0);
    assert(customos_driver_registry_self_check() == 1);
    return 0;
}
