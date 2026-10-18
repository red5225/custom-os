#include "range_overlap.h"
#include <stddef.h>
#include <stdint.h>

int ranges_overlap_u64(uint64_t a, uint64_t alen, uint64_t b, uint64_t blen) { if (alen == 0 || blen == 0) return 0; return a <= b ? b - a < alen : a - b < blen; }
