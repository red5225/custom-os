#ifndef CUSTOMOS_STORAGE_DIRECTORY_INDEX_H
#define CUSTOMOS_STORAGE_DIRECTORY_INDEX_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_directory_index_init(void);
unsigned long customos_directory_index_capabilities(void);
const char *customos_directory_index_name(void);
int customos_directory_index_self_check(void);

#endif
