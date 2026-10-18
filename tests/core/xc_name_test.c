#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "xc_name.h"
int main(void) { if (!(xc_name_equal("program", "program") && !xc_name_equal("XC", "xc") && !xc_name_equal(NULL, "x"))) { fprintf(stderr, "xc_name test failed\n"); return 1; } return 0; }
