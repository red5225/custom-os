#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_xc_diagnostics_name(), "xc_diagnostics") == 0);
    assert(customos_xc_diagnostics_capabilities() == 78UL);
    assert(customos_xc_diagnostics_self_check() == 0);
    assert(customos_xc_diagnostics_init() == 0);
    assert(customos_xc_diagnostics_self_check() == 1);
    return 0;
}
