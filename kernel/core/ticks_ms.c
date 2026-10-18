#include "ticks_ms.h"
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

uint64_t ticks_to_milliseconds(uint64_t ticks, uint32_t hz) { if (hz == 0) return UINT64_MAX; uint64_t whole = ticks / hz; uint64_t rem = ticks % hz; if (whole > UINT64_MAX / 1000u) return UINT64_MAX; return whole * 1000u + (rem * 1000u) / hz; }
