#ifndef CUSTOMOS_SECURITY_SECRET_REDACTION_H
#define CUSTOMOS_SECURITY_SECRET_REDACTION_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_secret_redaction_init(void);
unsigned long customos_secret_redaction_capabilities(void);
const char *customos_secret_redaction_name(void);
int customos_secret_redaction_self_check(void);

#endif
