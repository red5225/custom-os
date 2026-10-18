#include "block_range.h"
#include <stddef.h>
#include <stdint.h>

int block_range_valid(uint64_t start, uint64_t count, uint64_t total) { return start <= total && count <= total - start; }
