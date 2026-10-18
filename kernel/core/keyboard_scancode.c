#include "keyboard_scancode.h"
#include <stddef.h>
#include <stdint.h>

char keyboard_set1_ascii(uint8_t scan) { switch (scan) { case 0x1e: return 'a'; case 0x30: return 'b'; case 0x2e: return 'c'; case 0x39: return ' '; default: return 0; } }
