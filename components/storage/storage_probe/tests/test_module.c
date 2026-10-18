#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_storage_probe_name(), "storage_probe") == 0);
    assert(customos_storage_probe_capabilities() == 47UL);
    assert(customos_storage_probe_self_check() == 0);
    assert(customos_storage_probe_init() == 0);
    assert(customos_storage_probe_self_check() == 1);
    return 0;
}
