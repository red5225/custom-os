#include "fat_eoc.h"
#include <stddef.h>
#include <stdint.h>

int fat32_is_end_of_chain(uint32_t cluster) { return cluster >= 0x0ffffff8u && cluster <= 0x0fffffffu; }
