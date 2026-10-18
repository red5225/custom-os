#include "identifier_continue.h"
#include <stddef.h>
#include <stdint.h>

int ascii_identifier_continue(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9'); }
