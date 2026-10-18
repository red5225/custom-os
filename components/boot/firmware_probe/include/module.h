#ifndef CUSTOMOS_BOOT_FIRMWARE_PROBE_H
#define CUSTOMOS_BOOT_FIRMWARE_PROBE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_firmware_probe_init(void);
unsigned long customos_firmware_probe_capabilities(void);
const char *customos_firmware_probe_name(void);
int customos_firmware_probe_self_check(void);

#endif
