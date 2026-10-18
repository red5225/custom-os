#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_write_queue_name(), "write_queue") == 0);
    assert(customos_write_queue_capabilities() == 49UL);
    assert(customos_write_queue_self_check() == 0);
    assert(customos_write_queue_init() == 0);
    assert(customos_write_queue_self_check() == 1);
    return 0;
}
