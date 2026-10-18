#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_xc_values_name(), "xc_values") == 0);
    assert(customos_xc_values_capabilities() == 73UL);
    assert(customos_xc_values_self_check() == 0);
    assert(customos_xc_values_init() == 0);
    assert(customos_xc_values_self_check() == 1);
    return 0;
}
