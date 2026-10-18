#include "mouse_packet.h"
#include <stddef.h>
#include <stdint.h>

int mouse_packet_has_sync(uint8_t first_byte) { return (first_byte & 0x08u) != 0; }
