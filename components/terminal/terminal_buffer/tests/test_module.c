#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_terminal_buffer_name(), "terminal_buffer") == 0);
    assert(customos_terminal_buffer_capabilities() == 61UL);
    assert(customos_terminal_buffer_self_check() == 0);
    assert(customos_terminal_buffer_init() == 0);
    assert(customos_terminal_buffer_self_check() == 1);
    return 0;
}
