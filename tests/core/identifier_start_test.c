#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "identifier_start.h"
int main(void) { if (!(ascii_identifier_start('x') && ascii_identifier_start('_') && !ascii_identifier_start('7'))) { fprintf(stderr, "identifier_start test failed\n"); return 1; } return 0; }
