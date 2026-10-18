#ifndef CUSTOMOS_KERNEL_SYSCALL_TABLE_H
#define CUSTOMOS_KERNEL_SYSCALL_TABLE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_syscall_table_init(void);
unsigned long customos_syscall_table_capabilities(void);
const char *customos_syscall_table_name(void);
int customos_syscall_table_self_check(void);

#endif
