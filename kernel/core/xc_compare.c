#include "xc_compare.h"
#include <stddef.h>
#include <stdint.h>

int xc_compare_i64(int64_t a, int64_t b) { return a < b ? -1 : a > b ? 1 : 0; }
