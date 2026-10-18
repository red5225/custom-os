#include "pit_divisor.h"
#include <stddef.h>
#include <stdint.h>

uint16_t pit_divisor_for_hz(uint32_t hz) { if (hz == 0) return 0; uint32_t value = 1193182u / hz; return value == 0 || value > 65535u ? 0 : (uint16_t)value; }
