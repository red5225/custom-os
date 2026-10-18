#ifndef CUSTOMOS_BOOT_BOOT_FALLBACK_H
#define CUSTOMOS_BOOT_BOOT_FALLBACK_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_boot_fallback_init(void);
unsigned long customos_boot_fallback_capabilities(void);
const char *customos_boot_fallback_name(void);
int customos_boot_fallback_self_check(void);

#endif
