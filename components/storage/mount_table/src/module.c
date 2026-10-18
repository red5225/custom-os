#include "module.h"

static int initialized;

int customos_mount_table_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_mount_table_capabilities(void) {
    return 46UL;
}

const char *customos_mount_table_name(void) {
    return "mount_table";
}

int customos_mount_table_self_check(void) {
    return initialized == 1 && customos_mount_table_capabilities() == 46UL;
}
