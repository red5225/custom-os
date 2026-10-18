#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "identifier_continue.h"
int main(void) { if (!(ascii_identifier_continue('x') && ascii_identifier_continue('7') && !ascii_identifier_continue('-'))) { fprintf(stderr, "identifier_continue test failed\n"); return 1; } return 0; }
