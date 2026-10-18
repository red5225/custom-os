#include "log_level.h"
#include <stddef.h>
#include <stdint.h>

int log_level_enabled(uint8_t configured, uint8_t message) { return configured <= 5 && message <= 5 && message <= configured; }
