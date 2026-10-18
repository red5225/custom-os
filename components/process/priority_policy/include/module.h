#ifndef CUSTOMOS_PROCESS_PRIORITY_POLICY_H
#define CUSTOMOS_PROCESS_PRIORITY_POLICY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_priority_policy_init(void);
unsigned long customos_priority_policy_capabilities(void);
const char *customos_priority_policy_name(void);
int customos_priority_policy_self_check(void);

#endif
