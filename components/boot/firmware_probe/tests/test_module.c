#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_firmware_probe_name(), "firmware_probe") == 0);
    assert(customos_firmware_probe_capabilities() == 9UL);
    assert(customos_firmware_probe_self_check() == 0);
    assert(customos_firmware_probe_init() == 0);
    assert(customos_firmware_probe_self_check() == 1);
    return 0;
}
