#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_memory_map_name(), "memory_map") == 0);
    assert(customos_memory_map_capabilities() == 24UL);
    assert(customos_memory_map_self_check() == 0);
    assert(customos_memory_map_init() == 0);
    assert(customos_memory_map_self_check() == 1);
    return 0;
}
