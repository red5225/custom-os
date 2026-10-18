#include "crc32.h"
#include <stddef.h>
#include <stdint.h>

uint32_t crc32_update_byte(uint32_t crc, uint8_t byte) { crc ^= byte; for (unsigned i = 0; i < 8; ++i) crc = (crc >> 1) ^ (0xedb88320u & (uint32_t)-(int32_t)(crc & 1u)); return crc; }
