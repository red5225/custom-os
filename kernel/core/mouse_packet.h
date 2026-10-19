#ifndef CUSTOMOS_MOUSE_PACKET_H
#define CUSTOMOS_MOUSE_PACKET_H

#include <stdint.h>

#define MOUSE_PACKET_BYTES 3u

#define MOUSE_BUTTON_LEFT   0x01u
#define MOUSE_BUTTON_RIGHT  0x02u
#define MOUSE_BUTTON_MIDDLE 0x04u

struct mouse_packet {
    int16_t x;
    int16_t y;
    uint8_t buttons;
    uint8_t x_overflow;
    uint8_t y_overflow;
};

/* A valid first PS/2 packet byte has bit 3 set. */
int mouse_packet_has_sync(uint8_t first_byte);

/*
 * Decode a standard three-byte PS/2 mouse packet.
 * Returns 1 on success, 0 if the packet pointer is null or the sync bit is
 * absent. On failure, out is left unchanged. x/y are signed device deltas;
 * the caller decides how to map device coordinates to screen coordinates.
 */
int mouse_packet_decode(const uint8_t packet[MOUSE_PACKET_BYTES],
                        struct mouse_packet *out);

#endif
