#ifndef CUSTOMOS_SECURITY_INTEGRITY_MANIFEST_H
#define CUSTOMOS_SECURITY_INTEGRITY_MANIFEST_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_integrity_manifest_init(void);
unsigned long customos_integrity_manifest_capabilities(void);
const char *customos_integrity_manifest_name(void);
int customos_integrity_manifest_self_check(void);

#endif
