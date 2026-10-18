#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_system_clock_name(), "system_clock") == 0);
    assert(customos_system_clock_capabilities() == 13UL);
    assert(customos_system_clock_self_check() == 0);
    assert(customos_system_clock_init() == 0);
    assert(customos_system_clock_self_check() == 1);
    return 0;
}
