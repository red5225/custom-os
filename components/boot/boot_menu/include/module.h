#ifndef CUSTOMOS_BOOT_BOOT_MENU_H
#define CUSTOMOS_BOOT_BOOT_MENU_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_boot_menu_init(void);
unsigned long customos_boot_menu_capabilities(void);
const char *customos_boot_menu_name(void);
int customos_boot_menu_self_check(void);

#endif
