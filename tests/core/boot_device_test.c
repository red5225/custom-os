#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "boot_device.h"
int main(void) { if (!(boot_device_is_uefi(0x49464555u) && !boot_device_is_uefi(0))) { fprintf(stderr, "boot_device test failed\n"); return 1; } return 0; }
