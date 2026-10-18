#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "crc32.h"
int main(void) { if (!(crc32_update_byte(0xffffffffu, 'a') != 0xffffffffu)) { fprintf(stderr, "crc32 test failed\n"); return 1; } return 0; }
