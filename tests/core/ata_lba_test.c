#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "ata_lba.h"
int main(void) { if (!(ata_lba28_valid(0x0fffffffu) && !ata_lba28_valid(0x10000000u))) { fprintf(stderr, "ata_lba test failed\n"); return 1; } return 0; }
