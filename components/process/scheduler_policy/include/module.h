#ifndef CUSTOMOS_PROCESS_SCHEDULER_POLICY_H
#define CUSTOMOS_PROCESS_SCHEDULER_POLICY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_scheduler_policy_init(void);
unsigned long customos_scheduler_policy_capabilities(void);
const char *customos_scheduler_policy_name(void);
int customos_scheduler_policy_self_check(void);

#endif
