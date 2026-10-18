#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_physical_pages_name(), "physical_pages") == 0);
    assert(customos_physical_pages_capabilities() == 21UL);
    assert(customos_physical_pages_self_check() == 0);
    assert(customos_physical_pages_init() == 0);
    assert(customos_physical_pages_self_check() == 1);
    return 0;
}
