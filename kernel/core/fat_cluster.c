#include "fat_cluster.h"
#include <stddef.h>
#include <stdint.h>

int fat32_cluster_valid(uint32_t cluster) { return cluster >= 2 && cluster < 0x0ffffff7u; }
