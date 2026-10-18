#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_heap_policy_name(), "heap_policy") == 0);
    assert(customos_heap_policy_capabilities() == 23UL);
    assert(customos_heap_policy_self_check() == 0);
    assert(customos_heap_policy_init() == 0);
    assert(customos_heap_policy_self_check() == 1);
    return 0;
}
