#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_virtual_pages_name(), "virtual_pages") == 0);
    assert(customos_virtual_pages_capabilities() == 22UL);
    assert(customos_virtual_pages_self_check() == 0);
    assert(customos_virtual_pages_init() == 0);
    assert(customos_virtual_pages_self_check() == 1);
    return 0;
}
