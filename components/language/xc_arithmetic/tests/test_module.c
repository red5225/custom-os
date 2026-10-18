#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_xc_arithmetic_name(), "xc_arithmetic") == 0);
    assert(customos_xc_arithmetic_capabilities() == 75UL);
    assert(customos_xc_arithmetic_self_check() == 0);
    assert(customos_xc_arithmetic_init() == 0);
    assert(customos_xc_arithmetic_self_check() == 1);
    return 0;
}
