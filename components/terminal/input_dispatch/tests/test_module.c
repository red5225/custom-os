#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_input_dispatch_name(), "input_dispatch") == 0);
    assert(customos_input_dispatch_capabilities() == 68UL);
    assert(customos_input_dispatch_self_check() == 0);
    assert(customos_input_dispatch_init() == 0);
    assert(customos_input_dispatch_self_check() == 1);
    return 0;
}
