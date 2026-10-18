#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "mouse_packet.h"
int main(void) { if (!(mouse_packet_has_sync(0x08) && mouse_packet_has_sync(0x09) && !mouse_packet_has_sync(0x00))) { fprintf(stderr, "mouse_packet test failed\n"); return 1; } return 0; }
