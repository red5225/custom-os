#include "interrupt_vector.h"
#include <stddef.h>
#include <stdint.h>

int interrupt_vector_is_hardware(uint8_t vector) { return vector >= 32; }
