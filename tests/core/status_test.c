#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "status.h"
int main(void) { if (!(status_is_error(-1) && !status_is_error(0) && !status_is_error(1))) { fprintf(stderr, "status test failed\n"); return 1; } return 0; }
