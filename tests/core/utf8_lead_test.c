#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "utf8_lead.h"
int main(void) { if (!(utf8_lead_length(0x41) == 1 && utf8_lead_length(0xc2) == 2 && utf8_lead_length(0xf0) == 4 && utf8_lead_length(0xc0) == 0)) { fprintf(stderr, "utf8_lead test failed\n"); return 1; } return 0; }
