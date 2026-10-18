#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_xc_module_loader_name(), "xc_module_loader") == 0);
    assert(customos_xc_module_loader_capabilities() == 80UL);
    assert(customos_xc_module_loader_self_check() == 0);
    assert(customos_xc_module_loader_init() == 0);
    assert(customos_xc_module_loader_self_check() == 1);
    return 0;
}
