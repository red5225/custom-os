#include "memory_range.h"
#include <stddef.h>
#include <stdint.h>

int memory_range_contains(uint64_t base, uint64_t length, uint64_t address) { return length != 0 && address >= base && address - base < length; }
