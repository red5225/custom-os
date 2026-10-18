#ifndef CUSTOMOS_BOOT_BOOT_LOGGING_H
#define CUSTOMOS_BOOT_BOOT_LOGGING_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_boot_logging_init(void);
unsigned long customos_boot_logging_capabilities(void);
const char *customos_boot_logging_name(void);
int customos_boot_logging_self_check(void);

#endif
