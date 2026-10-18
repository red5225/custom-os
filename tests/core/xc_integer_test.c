#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "xc_integer.h"
int main(void) { int64_t number = 0; if (!(xc_parse_integer("-42", &number) && number == -42 && !xc_parse_integer("4x", &number) && xc_parse_integer("9223372036854775807", &number))) { fprintf(stderr, "xc_integer test failed\n"); return 1; } return 0; }
