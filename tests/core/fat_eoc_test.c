#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "fat_eoc.h"
int main(void) { if (!(fat32_is_end_of_chain(0x0ffffff8u) && fat32_is_end_of_chain(0x0fffffffu) && !fat32_is_end_of_chain(0x0ffffff7u))) { fprintf(stderr, "fat_eoc test failed\n"); return 1; } return 0; }
