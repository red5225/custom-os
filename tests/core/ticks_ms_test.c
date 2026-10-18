#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "ticks_ms.h"
int main(void) { if (!(ticks_to_milliseconds(2, 100) == 20 && ticks_to_milliseconds(1, 0) == UINT64_MAX)) { fprintf(stderr, "ticks_ms test failed\n"); return 1; } return 0; }
