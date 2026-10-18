#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "power2.h"
int main(void) { if (!(is_power_of_two_u64(4096) && !is_power_of_two_u64(0) && !is_power_of_two_u64(12))) { fprintf(stderr, "power2 test failed\n"); return 1; } return 0; }
