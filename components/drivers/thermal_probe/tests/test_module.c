#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_thermal_probe_name(), "thermal_probe") == 0);
    assert(customos_thermal_probe_capabilities() == 59UL);
    assert(customos_thermal_probe_self_check() == 0);
    assert(customos_thermal_probe_init() == 0);
    assert(customos_thermal_probe_self_check() == 1);
    return 0;
}
