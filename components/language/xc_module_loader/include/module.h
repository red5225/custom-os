#ifndef CUSTOMOS_LANGUAGE_XC_MODULE_LOADER_H
#define CUSTOMOS_LANGUAGE_XC_MODULE_LOADER_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_xc_module_loader_init(void);
unsigned long customos_xc_module_loader_capabilities(void);
const char *customos_xc_module_loader_name(void);
int customos_xc_module_loader_self_check(void);

#endif
