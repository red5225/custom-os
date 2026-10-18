#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "log_level.h"
int main(void) { if (!(log_level_enabled(3, 2) && !log_level_enabled(2, 3) && !log_level_enabled(9, 1))) { fprintf(stderr, "log_level test failed\n"); return 1; } return 0; }
