#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "mouse_delta.h"
int main(void) { if (!(mouse_decode_delta(0x7f) == 127 && mouse_decode_delta(0x80) == -128 && mouse_decode_delta(0xff) == -1)) { fprintf(stderr, "mouse_delta test failed\n"); return 1; } return 0; }
