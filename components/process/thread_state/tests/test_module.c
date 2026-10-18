#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_thread_state_name(), "thread_state") == 0);
    assert(customos_thread_state_capabilities() == 33UL);
    assert(customos_thread_state_self_check() == 0);
    assert(customos_thread_state_init() == 0);
    assert(customos_thread_state_self_check() == 1);
    return 0;
}
