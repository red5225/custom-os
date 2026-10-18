#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "pit_divisor.h"
int main(void) { if (!(pit_divisor_for_hz(100) > 0 && pit_divisor_for_hz(0) == 0 && pit_divisor_for_hz(1) == 0)) { fprintf(stderr, "pit_divisor test failed\n"); return 1; } return 0; }
