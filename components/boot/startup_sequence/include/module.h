#ifndef CUSTOMOS_BOOT_STARTUP_SEQUENCE_H
#define CUSTOMOS_BOOT_STARTUP_SEQUENCE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_startup_sequence_init(void);
unsigned long customos_startup_sequence_capabilities(void);
const char *customos_startup_sequence_name(void);
int customos_startup_sequence_self_check(void);

#endif
