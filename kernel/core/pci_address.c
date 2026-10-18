#include "pci_address.h"
#include <stddef.h>
#include <stdint.h>

uint32_t pci_config_address(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) { if (device >= 32 || function >= 8) return 0; return 0x80000000u | ((uint32_t)bus << 16) | ((uint32_t)device << 11) | ((uint32_t)function << 8) | (offset & 0xfcu); }
