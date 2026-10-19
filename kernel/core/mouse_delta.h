#ifndef CUSTOMOS_MOUSE_DELTA_H
#define CUSTOMOS_MOUSE_DELTA_H

#include <stdint.h>

/*
 * Decode a PS/2 mouse movement byte as a signed 8-bit delta.
 * This is only the byte-level decode; packet overflow bits are handled by
 * mouse_packet_decode() and should not be interpreted as ordinary motion.
 */
int16_t mouse_decode_delta(uint8_t raw);

#endif
