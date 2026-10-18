#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "range_overlap.h"
int main(void) { if (!(ranges_overlap_u64(100, 20, 119, 5) && !ranges_overlap_u64(100, 20, 120, 5) && !ranges_overlap_u64(0, 0, 0, 4))) { fprintf(stderr, "range_overlap test failed\n"); return 1; } return 0; }
