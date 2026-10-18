#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_integrity_manifest_name(), "integrity_manifest") == 0);
    assert(customos_integrity_manifest_capabilities() == 91UL);
    assert(customos_integrity_manifest_self_check() == 0);
    assert(customos_integrity_manifest_init() == 0);
    assert(customos_integrity_manifest_self_check() == 1);
    return 0;
}
