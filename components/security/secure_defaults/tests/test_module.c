#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_secure_defaults_name(), "secure_defaults") == 0);
    assert(customos_secure_defaults_capabilities() == 94UL);
    assert(customos_secure_defaults_self_check() == 0);
    assert(customos_secure_defaults_init() == 0);
    assert(customos_secure_defaults_self_check() == 1);
    return 0;
}
