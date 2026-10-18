#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "exception_error.h"
int main(void) { if (!(exception_has_error_code(14) && exception_has_error_code(8) && !exception_has_error_code(3))) { fprintf(stderr, "exception_error test failed\n"); return 1; } return 0; }
