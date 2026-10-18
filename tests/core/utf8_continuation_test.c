#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "utf8_continuation.h"
int main(void) { if (!(utf8_is_continuation(0x80) && utf8_is_continuation(0xbf) && !utf8_is_continuation(0xc2))) { fprintf(stderr, "utf8_continuation test failed\n"); return 1; } return 0; }
