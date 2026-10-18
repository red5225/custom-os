#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_syscall_table_name(), "syscall_table") == 0);
    assert(customos_syscall_table_capabilities() == 16UL);
    assert(customos_syscall_table_self_check() == 0);
    assert(customos_syscall_table_init() == 0);
    assert(customos_syscall_table_self_check() == 1);
    return 0;
}
