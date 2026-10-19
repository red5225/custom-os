#include <stdint.h>
#include <stdio.h>
#include "mouse_packet.h"

int main(void) {
    struct mouse_packet out = { 11, 22, 3, 0, 0 };
    const uint8_t normal[MOUSE_PACKET_BYTES] = { 0x19u, 0x7fu, 0xffu };
    const uint8_t overflow[MOUSE_PACKET_BYTES] = { 0xc8u, 0x80u, 0x01u };
    const uint8_t bad_sync[MOUSE_PACKET_BYTES] = { 0x00u, 0x01u, 0x02u };

    if (!mouse_packet_has_sync(0x08u) ||
        !mouse_packet_has_sync(0x19u) ||
        mouse_packet_has_sync(0x00u) ||
        mouse_packet_has_sync(0x07u)) {
        fputs("mouse_packet sync test failed\n", stderr);
        return 1;
    }

    if (!mouse_packet_decode(normal, &out) ||
        out.x != 127 || out.y != -1 || out.buttons != 1u ||
        out.x_overflow != 0u || out.y_overflow != 0u) {
        fputs("mouse_packet normal decode failed\n", stderr);
        return 1;
    }

    if (!mouse_packet_decode(overflow, &out) ||
        out.x != -128 || out.y != 1 || out.x_overflow != 1u ||
        out.y_overflow != 1u || out.buttons != 0u) {
        fputs("mouse_packet overflow decode failed\n", stderr);
        return 1;
    }

    out.x = 11;
    out.y = 22;
    if (mouse_packet_decode(bad_sync, &out) || out.x != 11 || out.y != 22) {
        fputs("mouse_packet invalid packet handling failed\n", stderr);
        return 1;
    }
    if (mouse_packet_decode(0, &out) || mouse_packet_decode(normal, 0)) {
        fputs("mouse_packet null argument handling failed\n", stderr);
        return 1;
    }

    return 0;
}
