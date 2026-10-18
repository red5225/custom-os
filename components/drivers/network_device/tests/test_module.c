#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_network_device_name(), "network_device") == 0);
    assert(customos_network_device_capabilities() == 57UL);
    assert(customos_network_device_self_check() == 0);
    assert(customos_network_device_init() == 0);
    assert(customos_network_device_self_check() == 1);
    return 0;
}
