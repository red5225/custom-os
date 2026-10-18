#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_hash_registry_name(), "hash_registry") == 0);
    assert(customos_hash_registry_capabilities() == 92UL);
    assert(customos_hash_registry_self_check() == 0);
    assert(customos_hash_registry_init() == 0);
    assert(customos_hash_registry_self_check() == 1);
    return 0;
}
