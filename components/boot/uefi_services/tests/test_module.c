#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_uefi_services_name(), "uefi_services") == 0);
    assert(customos_uefi_services_capabilities() == 2UL);
    assert(customos_uefi_services_self_check() == 0);
    assert(customos_uefi_services_init() == 0);
    assert(customos_uefi_services_self_check() == 1);
    return 0;
}
