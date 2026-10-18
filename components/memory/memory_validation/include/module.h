#ifndef CUSTOMOS_MEMORY_MEMORY_VALIDATION_H
#define CUSTOMOS_MEMORY_MEMORY_VALIDATION_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_memory_validation_init(void);
unsigned long customos_memory_validation_capabilities(void);
const char *customos_memory_validation_name(void);
int customos_memory_validation_self_check(void);

#endif
