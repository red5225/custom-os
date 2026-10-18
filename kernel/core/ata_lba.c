#include "ata_lba.h"
#include <stddef.h>
#include <stdint.h>

int ata_lba28_valid(uint32_t lba) { return lba < 0x10000000u; }
