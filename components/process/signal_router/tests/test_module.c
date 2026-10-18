#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_signal_router_name(), "signal_router") == 0);
    assert(customos_signal_router_capabilities() == 36UL);
    assert(customos_signal_router_self_check() == 0);
    assert(customos_signal_router_init() == 0);
    assert(customos_signal_router_self_check() == 1);
    return 0;
}
