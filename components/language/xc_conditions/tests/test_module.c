#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_xc_conditions_name(), "xc_conditions") == 0);
    assert(customos_xc_conditions_capabilities() == 76UL);
    assert(customos_xc_conditions_self_check() == 0);
    assert(customos_xc_conditions_init() == 0);
    assert(customos_xc_conditions_self_check() == 1);
    return 0;
}
