#ifndef CUSTOMOS_LANGUAGE_XC_PARSER_H
#define CUSTOMOS_LANGUAGE_XC_PARSER_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_xc_parser_init(void);
unsigned long customos_xc_parser_capabilities(void);
const char *customos_xc_parser_name(void);
int customos_xc_parser_self_check(void);

#endif
