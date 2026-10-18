#include "color.h"
#include <stddef.h>
#include <stdint.h>

uint32_t color_rgb888(uint8_t red, uint8_t green, uint8_t blue) { return ((uint32_t)red << 16) | ((uint32_t)green << 8) | blue; }
