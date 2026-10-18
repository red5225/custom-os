#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "color.h"
int main(void) { if (!(color_rgb888(0x12, 0x34, 0x56) == 0x123456u)) { fprintf(stderr, "color test failed\n"); return 1; } return 0; }
