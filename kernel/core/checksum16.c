#include "checksum16.h"
#include <stddef.h>
#include <stdint.h>

uint16_t checksum16_add(uint16_t sum, uint16_t word) { uint32_t value = (uint32_t)sum + word; return (uint16_t)((value & 0xffffu) + (value >> 16)); }
