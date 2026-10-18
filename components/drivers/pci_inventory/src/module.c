#include "module.h"

static int initialized;

int customos_pci_inventory_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_pci_inventory_capabilities(void) {
    return 51UL;
}

const char *customos_pci_inventory_name(void) {
    return "pci_inventory";
}

int customos_pci_inventory_self_check(void) {
    return initialized == 1 && customos_pci_inventory_capabilities() == 51UL;
}
