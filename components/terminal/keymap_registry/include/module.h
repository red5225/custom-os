#ifndef CUSTOMOS_TERMINAL_KEYMAP_REGISTRY_H
#define CUSTOMOS_TERMINAL_KEYMAP_REGISTRY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_keymap_registry_init(void);
unsigned long customos_keymap_registry_capabilities(void);
const char *customos_keymap_registry_name(void);
int customos_keymap_registry_self_check(void);

#endif
