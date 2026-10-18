#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "pci_location.h"
int main(void) { if (!(pci_location_valid(31, 7) && !pci_location_valid(32, 0) && !pci_location_valid(1, 8))) { fprintf(stderr, "pci_location test failed\n"); return 1; } return 0; }
