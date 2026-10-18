#ifndef CUSTOMOS_SECURITY_AUDIT_EVENTS_H
#define CUSTOMOS_SECURITY_AUDIT_EVENTS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_audit_events_init(void);
unsigned long customos_audit_events_capabilities(void);
const char *customos_audit_events_name(void);
int customos_audit_events_self_check(void);

#endif
