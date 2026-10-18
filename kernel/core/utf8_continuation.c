#include "utf8_continuation.h"
#include <stddef.h>
#include <stdint.h>

int utf8_is_continuation(uint8_t byte) { return (byte & 0xc0u) == 0x80u; }
