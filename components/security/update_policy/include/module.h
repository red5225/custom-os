#ifndef CUSTOMOS_SECURITY_UPDATE_POLICY_H
#define CUSTOMOS_SECURITY_UPDATE_POLICY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_update_policy_init(void);
unsigned long customos_update_policy_capabilities(void);
const char *customos_update_policy_name(void);
int customos_update_policy_self_check(void);

#endif
