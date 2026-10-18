#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "block_range.h"
int main(void) { if (!(block_range_valid(90, 10, 100) && !block_range_valid(91, 10, 100) && block_range_valid(100, 0, 100))) { fprintf(stderr, "block_range test failed\n"); return 1; } return 0; }
