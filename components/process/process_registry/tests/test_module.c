#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_process_registry_name(), "process_registry") == 0);
    assert(customos_process_registry_capabilities() == 31UL);
    assert(customos_process_registry_self_check() == 0);
    assert(customos_process_registry_init() == 0);
    assert(customos_process_registry_self_check() == 1);
    return 0;
}
