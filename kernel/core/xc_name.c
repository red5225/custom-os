#include "xc_name.h"
#include <stddef.h>
#include <stdint.h>

int xc_name_equal(const char *a, const char *b) { if (a == NULL || b == NULL) return 0; while (*a && *b && *a == *b) { ++a; ++b; } return *a == '\0' && *b == '\0'; }
