#ifndef CUSTOMOS_STORAGE_MOUNT_TABLE_H
#define CUSTOMOS_STORAGE_MOUNT_TABLE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_mount_table_init(void);
unsigned long customos_mount_table_capabilities(void);
const char *customos_mount_table_name(void);
int customos_mount_table_self_check(void);

#endif
