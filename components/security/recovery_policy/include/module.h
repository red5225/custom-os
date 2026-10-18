#ifndef CUSTOMOS_SECURITY_RECOVERY_POLICY_H
#define CUSTOMOS_SECURITY_RECOVERY_POLICY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_recovery_policy_init(void);
unsigned long customos_recovery_policy_capabilities(void);
const char *customos_recovery_policy_name(void);
int customos_recovery_policy_self_check(void);

#endif
