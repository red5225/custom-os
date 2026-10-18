#include "identifier_start.h"
#include <stddef.h>
#include <stdint.h>

int ascii_identifier_start(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
