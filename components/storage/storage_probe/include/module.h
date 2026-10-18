#ifndef CUSTOMOS_STORAGE_STORAGE_PROBE_H
#define CUSTOMOS_STORAGE_STORAGE_PROBE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_storage_probe_init(void);
unsigned long customos_storage_probe_capabilities(void);
const char *customos_storage_probe_name(void);
int customos_storage_probe_self_check(void);

#endif
