#include "lcg.h"
#include <stddef.h>
#include <stdint.h>

uint32_t lcg_next(uint32_t state) { return state * 1664525u + 1013904223u; }
