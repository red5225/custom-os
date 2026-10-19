#include <stdint.h>
#include <stdio.h>
#include "mouse_delta.h"

int main(void) {
    if (mouse_decode_delta(0x00u) != 0 ||
        mouse_decode_delta(0x01u) != 1 ||
        mouse_decode_delta(0x7fu) != 127 ||
        mouse_decode_delta(0x80u) != -128 ||
        mouse_decode_delta(0xffu) != -1 ||
        mouse_decode_delta(0xfeu) != -2) {
        fputs("mouse_delta test failed\n", stderr);
        return 1;
    }
    return 0;
}
