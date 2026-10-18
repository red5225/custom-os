#ifndef CUSTOMOS_DRIVERS_PCI_INVENTORY_H
#define CUSTOMOS_DRIVERS_PCI_INVENTORY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_pci_inventory_init(void);
unsigned long customos_pci_inventory_capabilities(void);
const char *customos_pci_inventory_name(void);
int customos_pci_inventory_self_check(void);

#endif
