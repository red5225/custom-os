#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "bitmap_set.h"
int main(void) { uint8_t bits[2] = {0, 0}; if (!(bitmap_set(bits, 16, 9) && (bits[1] & 2u) != 0 && !bitmap_set(bits, 16, 16))) { fprintf(stderr, "bitmap_set test failed\n"); return 1; } return 0; }
