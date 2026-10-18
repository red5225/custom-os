#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_page_permissions_name(), "page_permissions") == 0);
    assert(customos_page_permissions_capabilities() == 25UL);
    assert(customos_page_permissions_self_check() == 0);
    assert(customos_page_permissions_init() == 0);
    assert(customos_page_permissions_self_check() == 1);
    return 0;
}
