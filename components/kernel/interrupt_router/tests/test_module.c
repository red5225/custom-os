#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_interrupt_router_name(), "interrupt_router") == 0);
    assert(customos_interrupt_router_capabilities() == 15UL);
    assert(customos_interrupt_router_self_check() == 0);
    assert(customos_interrupt_router_init() == 0);
    assert(customos_interrupt_router_self_check() == 1);
    return 0;
}
