#ifndef CUSTOMOS_STORAGE_FILESYSTEM_CHECK_H
#define CUSTOMOS_STORAGE_FILESYSTEM_CHECK_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_filesystem_check_init(void);
unsigned long customos_filesystem_check_capabilities(void);
const char *customos_filesystem_check_name(void);
int customos_filesystem_check_self_check(void);

#endif
