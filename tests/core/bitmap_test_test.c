#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "bitmap_test.h"
int main(void) { uint8_t bits[2] = {0, 2}; if (!(bitmap_test(bits, 16, 9) && !bitmap_test(bits, 16, 8) && !bitmap_test(bits, 16, 16))) { fprintf(stderr, "bitmap_test test failed\n"); return 1; } return 0; }
