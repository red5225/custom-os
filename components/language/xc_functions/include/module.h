#ifndef CUSTOMOS_LANGUAGE_XC_FUNCTIONS_H
#define CUSTOMOS_LANGUAGE_XC_FUNCTIONS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_xc_functions_init(void);
unsigned long customos_xc_functions_capabilities(void);
const char *customos_xc_functions_name(void);
int customos_xc_functions_self_check(void);

#endif
