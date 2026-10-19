#include "mouse_packet.h"
#include "mouse_delta.h"

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

    /*
     * The device's sign bits are authoritative. The low byte is still decoded
     * as signed two's-complement for a conventional PS/2 packet. If overflow
     * is set, consumers should ignore that axis delta rather than move by it.
     */
    decoded.x = mouse_decode_delta(packet[1]);
    decoded.y = mouse_decode_delta(packet[2]);

    *out = decoded;
    return 1;
}
