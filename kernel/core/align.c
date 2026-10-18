#include "align.h"
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

uint64_t align_up_u64(uint64_t value, uint64_t alignment) { if (alignment == 0 || (alignment & (alignment - 1)) != 0 || value > UINT64_MAX - (alignment - 1)) return UINT64_MAX; return (value + alignment - 1) & ~(alignment - 1); }
