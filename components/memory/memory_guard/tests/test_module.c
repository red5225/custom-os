#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_memory_guard_name(), "memory_guard") == 0);
    assert(customos_memory_guard_capabilities() == 29UL);
    assert(customos_memory_guard_self_check() == 0);
    assert(customos_memory_guard_init() == 0);
    assert(customos_memory_guard_self_check() == 1);
    return 0;
}
