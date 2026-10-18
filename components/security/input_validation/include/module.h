#ifndef CUSTOMOS_SECURITY_INPUT_VALIDATION_H
#define CUSTOMOS_SECURITY_INPUT_VALIDATION_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_input_validation_init(void);
unsigned long customos_input_validation_capabilities(void);
const char *customos_input_validation_name(void);
int customos_input_validation_self_check(void);

#endif
