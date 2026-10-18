#ifndef CUSTOMOS_STORAGE_FILE_METADATA_H
#define CUSTOMOS_STORAGE_FILE_METADATA_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_file_metadata_init(void);
unsigned long customos_file_metadata_capabilities(void);
const char *customos_file_metadata_name(void);
int customos_file_metadata_self_check(void);

#endif
