#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_battery_probe_name(), "battery_probe") == 0);
    assert(customos_battery_probe_capabilities() == 58UL);
    assert(customos_battery_probe_self_check() == 0);
    assert(customos_battery_probe_init() == 0);
    assert(customos_battery_probe_self_check() == 1);
    return 0;
}
