#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_xc_functions_name(), "xc_functions") == 0);
    assert(customos_xc_functions_capabilities() == 77UL);
    assert(customos_xc_functions_self_check() == 0);
    assert(customos_xc_functions_init() == 0);
    assert(customos_xc_functions_self_check() == 1);
    return 0;
}
