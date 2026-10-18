#include "keyboard_release.h"
#include <stddef.h>
#include <stdint.h>

int keyboard_scancode_is_release(uint8_t scan) { return (scan & 0x80u) != 0; }
