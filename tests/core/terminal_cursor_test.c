#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "terminal_cursor.h"
int main(void) { if (!(terminal_next_column(3, 80) == 4 && terminal_next_column(79, 80) == 0 && terminal_next_column(0, 0) == 0)) { fprintf(stderr, "terminal_cursor test failed\n"); return 1; } return 0; }
