#include "stack_top.h"
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

uint64_t kernel_stack_top(uint64_t base, uint64_t size) { if (size == 0 || base > UINT64_MAX - size) return 0; return base + size; }
