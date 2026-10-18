#include "identifier_continue.h"
#include <stddef.h>
#include <stdint.h>

int ascii_identifier_continue(char c) { return ascii_identifier_start(c) || (c >= '0' && c <= '9'); }
