#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_memory_stats_name(), "memory_stats") == 0);
    assert(customos_memory_stats_capabilities() == 27UL);
    assert(customos_memory_stats_self_check() == 0);
    assert(customos_memory_stats_init() == 0);
    assert(customos_memory_stats_self_check() == 1);
    return 0;
}
