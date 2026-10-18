#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_xc_variables_name(), "xc_variables") == 0);
    assert(customos_xc_variables_capabilities() == 74UL);
    assert(customos_xc_variables_self_check() == 0);
    assert(customos_xc_variables_init() == 0);
    assert(customos_xc_variables_self_check() == 1);
    return 0;
}
