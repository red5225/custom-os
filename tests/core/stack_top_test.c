#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "stack_top.h"
int main(void) { if (!(kernel_stack_top(0x1000, 0x2000) == 0x3000 && kernel_stack_top(UINT64_MAX - 1, 4) == 0 && kernel_stack_top(1, 0) == 0)) { fprintf(stderr, "stack_top test failed\n"); return 1; } return 0; }
