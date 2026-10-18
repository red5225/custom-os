#ifndef CUSTOMOS_MEMORY_VIRTUAL_PAGES_H
#define CUSTOMOS_MEMORY_VIRTUAL_PAGES_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_virtual_pages_init(void);
unsigned long customos_virtual_pages_capabilities(void);
const char *customos_virtual_pages_name(void);
int customos_virtual_pages_self_check(void);

#endif
