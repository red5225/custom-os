#ifndef CUSTOMOS_MEMORY_PAGE_PERMISSIONS_H
#define CUSTOMOS_MEMORY_PAGE_PERMISSIONS_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_page_permissions_init(void);
unsigned long customos_page_permissions_capabilities(void);
const char *customos_page_permissions_name(void);
int customos_page_permissions_self_check(void);

#endif
