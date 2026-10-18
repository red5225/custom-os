#ifndef CUSTOMOS_BOOT_BOOT_CONFIG_H
#define CUSTOMOS_BOOT_BOOT_CONFIG_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_boot_config_init(void);
unsigned long customos_boot_config_capabilities(void);
const char *customos_boot_config_name(void);
int customos_boot_config_self_check(void);

#endif
