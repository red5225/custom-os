#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "keyboard_scancode.h"
int main(void) { if (!(keyboard_set1_ascii(0x1e) == 'a' && keyboard_set1_ascii(0x2e) == 'c' && keyboard_set1_ascii(0x20) == 0)) { fprintf(stderr, "keyboard_scancode test failed\n"); return 1; } return 0; }
