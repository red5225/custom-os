#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "memory_range.h"
int main(void) { if (!(memory_range_contains(0x1000, 0x1000, 0x1fff) && !memory_range_contains(0x1000, 0x1000, 0x2000))) { fprintf(stderr, "memory_range test failed\n"); return 1; } return 0; }
