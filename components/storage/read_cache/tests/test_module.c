#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_read_cache_name(), "read_cache") == 0);
    assert(customos_read_cache_capabilities() == 48UL);
    assert(customos_read_cache_self_check() == 0);
    assert(customos_read_cache_init() == 0);
    assert(customos_read_cache_self_check() == 1);
    return 0;
}
