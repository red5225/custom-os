#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_command_router_name(), "command_router") == 0);
    assert(customos_command_router_capabilities() == 64UL);
    assert(customos_command_router_self_check() == 0);
    assert(customos_command_router_init() == 0);
    assert(customos_command_router_self_check() == 1);
    return 0;
}
