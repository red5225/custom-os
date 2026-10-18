#ifndef CUSTOMOS_LANGUAGE_XC_TOKENS_H
#define CUSTOMOS_LANGUAGE_XC_TOKENS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_xc_tokens_init(void);
unsigned long customos_xc_tokens_capabilities(void);
const char *customos_xc_tokens_name(void);
int customos_xc_tokens_self_check(void);

#endif
