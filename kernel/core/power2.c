#include "power2.h"
#include <stddef.h>
#include <stdint.h>

int is_power_of_two_u64(uint64_t value) { return value != 0 && (value & (value - 1)) == 0; }
