#ifndef CUSTOMOS_BOOT_LOADER_STATE_H
#define CUSTOMOS_BOOT_LOADER_STATE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_loader_state_init(void);
unsigned long customos_loader_state_capabilities(void);
const char *customos_loader_state_name(void);
int customos_loader_state_self_check(void);

#endif
