#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "boot_info.h"
int main(void) { if (!(boot_info_valid(0x42, 0x42, 1, 2, 64, 32) && !boot_info_valid(0x41, 0x42, 1, 2, 64, 32))) { fprintf(stderr, "boot_info test failed\n"); return 1; } return 0; }
