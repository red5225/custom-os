#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "checksum16.h"
int main(void) { if (!(checksum16_add(0xffffu, 1u) == 1u && checksum16_add(2u, 3u) == 5u)) { fprintf(stderr, "checksum16 test failed\n"); return 1; } return 0; }
