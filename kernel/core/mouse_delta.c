#include "mouse_delta.h"
#include <stddef.h>
#include <stdint.h>

int16_t mouse_decode_delta(uint8_t raw) { return raw < 128u ? (int16_t)raw : (int16_t)raw - 256; }
