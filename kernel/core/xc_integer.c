#include "xc_integer.h"
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

int xc_parse_integer(const char *text, int64_t *out) { if (text == NULL || out == NULL || *text == '\0') return 0; int neg = (*text == '-'); if (neg) ++text; if (*text == '\0') return 0; uint64_t value = 0; uint64_t limit = neg ? (uint64_t)INT64_MAX + 1u : (uint64_t)INT64_MAX; for (; *text; ++text) { if (*text < '0' || *text > '9') return 0; unsigned digit = (unsigned)(*text - '0'); if (value > (limit - digit) / 10u) return 0; value = value * 10u + digit; } *out = neg ? (value == (uint64_t)INT64_MAX + 1u ? INT64_MIN : -(int64_t)value) : (int64_t)value; return 1; }
