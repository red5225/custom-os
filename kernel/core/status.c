#include "status.h"
#include <stddef.h>
#include <stdint.h>

int status_is_error(int32_t status) { return status < 0; }
