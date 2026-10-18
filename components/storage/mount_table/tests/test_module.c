#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_mount_table_name(), "mount_table") == 0);
    assert(customos_mount_table_capabilities() == 46UL);
    assert(customos_mount_table_self_check() == 0);
    assert(customos_mount_table_init() == 0);
    assert(customos_mount_table_self_check() == 1);
    return 0;
}
