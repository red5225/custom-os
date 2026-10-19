#include "mouse_delta.h"

int16_t mouse_decode_delta(uint8_t raw) {
    /* Explicit sign extension avoids relying on implementation-defined casts. */
    if ((raw & 0x80u) != 0u) {
        return (int16_t)((int16_t)raw - 256);
    }
    return (int16_t)raw;
}
