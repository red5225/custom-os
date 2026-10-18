#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "keyboard_release.h"
int main(void) { if (!(keyboard_scancode_is_release(0x9e) && !keyboard_scancode_is_release(0x1e))) { fprintf(stderr, "keyboard_release test failed\n"); return 1; } return 0; }
