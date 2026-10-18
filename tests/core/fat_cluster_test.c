#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "fat_cluster.h"
int main(void) { if (!(fat32_cluster_valid(2) && fat32_cluster_valid(100) && !fat32_cluster_valid(0) && !fat32_cluster_valid(0x0ffffff7u))) { fprintf(stderr, "fat_cluster test failed\n"); return 1; } return 0; }
