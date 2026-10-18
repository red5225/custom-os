#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_wait_queue_name(), "wait_queue") == 0);
    assert(customos_wait_queue_capabilities() == 35UL);
    assert(customos_wait_queue_self_check() == 0);
    assert(customos_wait_queue_init() == 0);
    assert(customos_wait_queue_self_check() == 1);
    return 0;
}
