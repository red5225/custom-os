#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "framebuffer.h"
int main(void) { if (!(framebuffer_byte_offset(2, 3, 640, 640, 480) == ((size_t)3 * 640 + 2) * 4 && framebuffer_byte_offset(640, 0, 640, 480, 480) == SIZE_MAX)) { fprintf(stderr, "framebuffer test failed\n"); return 1; } return 0; }
