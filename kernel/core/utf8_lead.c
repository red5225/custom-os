#include "utf8_lead.h"
#include <stddef.h>
#include <stdint.h>

uint8_t utf8_lead_length(uint8_t byte) { if (byte < 0x80u) return 1; if (byte >= 0xc2u && byte <= 0xdfu) return 2; if (byte >= 0xe0u && byte <= 0xefu) return 3; if (byte >= 0xf0u && byte <= 0xf4u) return 4; return 0; }
