#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_filesystem_check_name(), "filesystem_check") == 0);
    assert(customos_filesystem_check_capabilities() == 50UL);
    assert(customos_filesystem_check_self_check() == 0);
    assert(customos_filesystem_check_init() == 0);
    assert(customos_filesystem_check_self_check() == 1);
    return 0;
}
