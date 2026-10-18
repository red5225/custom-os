#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_block_device_name(), "block_device") == 0);
    assert(customos_block_device_capabilities() == 41UL);
    assert(customos_block_device_self_check() == 0);
    assert(customos_block_device_init() == 0);
    assert(customos_block_device_self_check() == 1);
    return 0;
}
