#ifndef CUSTOMOS_LANGUAGE_XC_CONDITIONS_H
#define CUSTOMOS_LANGUAGE_XC_CONDITIONS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_xc_conditions_init(void);
unsigned long customos_xc_conditions_capabilities(void);
const char *customos_xc_conditions_name(void);
int customos_xc_conditions_self_check(void);

#endif
