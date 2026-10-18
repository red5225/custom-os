#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "xc_span.h"
int main(void) { if (!(xc_source_span_valid(4, 3, 7) && xc_source_span_valid(7, 0, 7) && !xc_source_span_valid(6, 2, 7))) { fprintf(stderr, "xc_span test failed\n"); return 1; } return 0; }
