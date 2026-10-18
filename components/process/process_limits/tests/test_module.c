#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_process_limits_name(), "process_limits") == 0);
    assert(customos_process_limits_capabilities() == 37UL);
    assert(customos_process_limits_self_check() == 0);
    assert(customos_process_limits_init() == 0);
    assert(customos_process_limits_self_check() == 1);
    return 0;
}
