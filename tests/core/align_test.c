#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "align.h"
int main(void) { if (!(align_up_u64(4097, 4096) == 8192 && align_up_u64(8, 3) == UINT64_MAX)) { fprintf(stderr, "align test failed\n"); return 1; } return 0; }
