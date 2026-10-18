#include "bitmap_set.h"
#include <stddef.h>
#include <stdint.h>

int bitmap_set(uint8_t *map, size_t bit_count, size_t bit) { if (map == NULL || bit >= bit_count) return 0; map[bit >> 3] |= (uint8_t)(1u << (bit & 7)); return 1; }
