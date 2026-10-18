#include "module.h"

static int initialized;

int customos_syscall_table_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_syscall_table_capabilities(void) {
    return 16UL;
}

const char *customos_syscall_table_name(void) {
    return "syscall_table";
}

int customos_syscall_table_self_check(void) {
    return initialized == 1 && customos_syscall_table_capabilities() == 16UL;
}
