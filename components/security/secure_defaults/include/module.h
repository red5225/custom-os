#ifndef CUSTOMOS_SECURITY_SECURE_DEFAULTS_H
#define CUSTOMOS_SECURITY_SECURE_DEFAULTS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_secure_defaults_init(void);
unsigned long customos_secure_defaults_capabilities(void);
const char *customos_secure_defaults_name(void);
int customos_secure_defaults_self_check(void);

#endif
