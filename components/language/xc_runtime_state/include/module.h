#ifndef CUSTOMOS_LANGUAGE_XC_RUNTIME_STATE_H
#define CUSTOMOS_LANGUAGE_XC_RUNTIME_STATE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_xc_runtime_state_init(void);
unsigned long customos_xc_runtime_state_capabilities(void);
const char *customos_xc_runtime_state_name(void);
int customos_xc_runtime_state_self_check(void);

#endif
