#include "exception_error.h"
#include <stddef.h>
#include <stdint.h>

int exception_has_error_code(uint8_t vector) { switch (vector) { case 8: case 10: case 11: case 12: case 13: case 14: case 17: case 21: case 29: case 30: return 1; default: return 0; } }
