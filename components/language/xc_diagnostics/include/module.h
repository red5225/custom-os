#ifndef CUSTOMOS_LANGUAGE_XC_DIAGNOSTICS_H
#define CUSTOMOS_LANGUAGE_XC_DIAGNOSTICS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_xc_diagnostics_init(void);
unsigned long customos_xc_diagnostics_capabilities(void);
const char *customos_xc_diagnostics_name(void);
int customos_xc_diagnostics_self_check(void);

#endif
