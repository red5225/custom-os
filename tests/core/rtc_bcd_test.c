#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "rtc_bcd.h"
int main(void) { uint8_t decoded = 0; if (!(rtc_bcd_to_binary(0x59, &decoded) && decoded == 59 && !rtc_bcd_to_binary(0x6a, &decoded))) { fprintf(stderr, "rtc_bcd test failed\n"); return 1; } return 0; }
