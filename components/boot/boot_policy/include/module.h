#ifndef CUSTOMOS_BOOT_BOOT_POLICY_H
#define CUSTOMOS_BOOT_BOOT_POLICY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_boot_policy_init(void);
unsigned long customos_boot_policy_capabilities(void);
const char *customos_boot_policy_name(void);
int customos_boot_policy_self_check(void);

#endif
