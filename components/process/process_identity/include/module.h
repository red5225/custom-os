#ifndef CUSTOMOS_PROCESS_PROCESS_IDENTITY_H
#define CUSTOMOS_PROCESS_PROCESS_IDENTITY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_process_identity_init(void);
unsigned long customos_process_identity_capabilities(void);
const char *customos_process_identity_name(void);
int customos_process_identity_self_check(void);

#endif
