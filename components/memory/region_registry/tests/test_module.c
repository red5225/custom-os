#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_region_registry_name(), "region_registry") == 0);
    assert(customos_region_registry_capabilities() == 28UL);
    assert(customos_region_registry_self_check() == 0);
    assert(customos_region_registry_init() == 0);
    assert(customos_region_registry_self_check() == 1);
    return 0;
}
