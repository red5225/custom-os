#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "lcg.h"
int main(void) { if (!(lcg_next(0) == 1013904223u && lcg_next(1) != lcg_next(2))) { fprintf(stderr, "lcg test failed\n"); return 1; } return 0; }
