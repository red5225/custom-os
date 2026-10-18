#ifndef CUSTOMOS_TERMINAL_SHELL_POLICY_H
#define CUSTOMOS_TERMINAL_SHELL_POLICY_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_shell_policy_init(void);
unsigned long customos_shell_policy_capabilities(void);
const char *customos_shell_policy_name(void);
int customos_shell_policy_self_check(void);

#endif
