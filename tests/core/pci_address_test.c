#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "pci_address.h"
int main(void) { if (!(pci_config_address(0, 1, 0, 0) == 0x80000800u && pci_config_address(0, 32, 0, 0) == 0)) { fprintf(stderr, "pci_address test failed\n"); return 1; } return 0; }
