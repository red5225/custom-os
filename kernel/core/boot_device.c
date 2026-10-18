#include "boot_device.h"
#include <stddef.h>
#include <stdint.h>

int boot_device_is_uefi(uint32_t signature) { return signature == 0x49464555u; }
