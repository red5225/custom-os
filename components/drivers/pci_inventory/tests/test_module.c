#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_pci_inventory_name(), "pci_inventory") == 0);
    assert(customos_pci_inventory_capabilities() == 51UL);
    assert(customos_pci_inventory_self_check() == 0);
    assert(customos_pci_inventory_init() == 0);
    assert(customos_pci_inventory_self_check() == 1);
    return 0;
}
