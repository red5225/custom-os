#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_memory_validation_name(), "memory_validation") == 0);
    assert(customos_memory_validation_capabilities() == 30UL);
    assert(customos_memory_validation_self_check() == 0);
    assert(customos_memory_validation_init() == 0);
    assert(customos_memory_validation_self_check() == 1);
    return 0;
}
