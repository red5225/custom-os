#ifndef CUSTOMOS_SECURITY_PERMISSION_POLICY_H
#define CUSTOMOS_SECURITY_PERMISSION_POLICY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_permission_policy_init(void);
unsigned long customos_permission_policy_capabilities(void);
const char *customos_permission_policy_name(void);
int customos_permission_policy_self_check(void);

#endif
