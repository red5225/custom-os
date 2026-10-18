#include "rtc_bcd.h"
#include <stddef.h>
#include <stdint.h>

int rtc_bcd_to_binary(uint8_t bcd, uint8_t *out) { if (out == NULL || (bcd & 0x0fu) > 9 || ((bcd >> 4) & 0x0fu) > 9) return 0; *out = (uint8_t)(((bcd >> 4) & 0x0fu) * 10u + (bcd & 0x0fu)); return 1; }
