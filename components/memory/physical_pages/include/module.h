#ifndef CUSTOMOS_MEMORY_PHYSICAL_PAGES_H
#define CUSTOMOS_MEMORY_PHYSICAL_PAGES_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_physical_pages_init(void);
unsigned long customos_physical_pages_capabilities(void);
const char *customos_physical_pages_name(void);
int customos_physical_pages_self_check(void);

#endif
