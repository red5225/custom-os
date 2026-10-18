#ifndef CUSTOMOS_LANGUAGE_XC_VARIABLES_H
#define CUSTOMOS_LANGUAGE_XC_VARIABLES_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_xc_variables_init(void);
unsigned long customos_xc_variables_capabilities(void);
const char *customos_xc_variables_name(void);
int customos_xc_variables_self_check(void);

#endif
