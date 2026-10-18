#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "interrupt_vector.h"
int main(void) { if (!(interrupt_vector_is_hardware(32) && !interrupt_vector_is_hardware(14))) { fprintf(stderr, "interrupt_vector test failed\n"); return 1; } return 0; }
