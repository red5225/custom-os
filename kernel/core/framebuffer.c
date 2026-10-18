#include "framebuffer.h"
#include <stddef.h>
#include <stdint.h>

size_t framebuffer_byte_offset(uint32_t x, uint32_t y, uint32_t pitch, uint32_t width, uint32_t height) { if (x >= width || y >= height || pitch < width) return SIZE_MAX; if ((size_t)y > (SIZE_MAX / 4u - x) / pitch) return SIZE_MAX; return ((size_t)y * pitch + x) * 4u; }
