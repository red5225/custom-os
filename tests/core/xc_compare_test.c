#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "xc_compare.h"
int main(void) { if (!(xc_compare_i64(-4, 2) == -1 && xc_compare_i64(5, 5) == 0 && xc_compare_i64(8, 1) == 1)) { fprintf(stderr, "xc_compare test failed\n"); return 1; } return 0; }
