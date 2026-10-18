#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_path_resolver_name(), "path_resolver") == 0);
    assert(customos_path_resolver_capabilities() == 43UL);
    assert(customos_path_resolver_self_check() == 0);
    assert(customos_path_resolver_init() == 0);
    assert(customos_path_resolver_self_check() == 1);
    return 0;
}
