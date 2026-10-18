#ifndef CUSTOMOS_BOOT_UEFI_SERVICES_H
#define CUSTOMOS_BOOT_UEFI_SERVICES_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_uefi_services_init(void);
unsigned long customos_uefi_services_capabilities(void);
const char *customos_uefi_services_name(void);
int customos_uefi_services_self_check(void);

#endif
