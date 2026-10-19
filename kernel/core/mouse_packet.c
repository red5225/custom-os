#include "mouse_packet.h"

static int16_t decode_signed_byte(uint8_t raw) {
    return (raw & 0x80u) ? (int16_t)((int16_t)raw - 256) : (int16_t)raw;
}

int mouse_packet_has_sync(uint8_t first_byte) {
    return (first_byte & 0x08u) != 0u;
}

int mouse_packet_decode(const uint8_t packet[MOUSE_PACKET_BYTES],
                        struct mouse_packet *out) {
    struct mouse_packet decoded;

    if (packet == 0 || out == 0 || !mouse_packet_has_sync(packet[0])) {
        return 0;
    }

    decoded.buttons = (uint8_t)(packet[0] & 0x07u);
    decoded.x_overflow = (uint8_t)((packet[0] >> 6u) & 0x01u);
    decoded.y_overflow = (uint8_t)((packet[0] >> 7u) & 0x01u);
    decoded.x = decode_signed_byte(packet[1]);
    decoded.y = decode_signed_byte(packet[2]);

    *out = decoded;
    return 1;
}
