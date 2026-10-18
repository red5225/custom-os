#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_loader_state_name(), "loader_state") == 0);
    assert(customos_loader_state_capabilities() == 6UL);
    assert(customos_loader_state_self_check() == 0);
    assert(customos_loader_state_init() == 0);
    assert(customos_loader_state_self_check() == 1);
    return 0;
}
