#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_terminal_status_name(), "terminal_status") == 0);
    assert(customos_terminal_status_capabilities() == 67UL);
    assert(customos_terminal_status_self_check() == 0);
    assert(customos_terminal_status_init() == 0);
    assert(customos_terminal_status_self_check() == 1);
    return 0;
}
