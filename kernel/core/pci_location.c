#include "pci_location.h"
#include <stddef.h>
#include <stdint.h>

int pci_location_valid(uint8_t device, uint8_t function) { return device < 32 && function < 8; }
