#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_volume_registry_name(), "volume_registry") == 0);
    assert(customos_volume_registry_capabilities() == 42UL);
    assert(customos_volume_registry_self_check() == 0);
    assert(customos_volume_registry_init() == 0);
    assert(customos_volume_registry_self_check() == 1);
    return 0;
}
