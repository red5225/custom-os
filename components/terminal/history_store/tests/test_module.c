#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_history_store_name(), "history_store") == 0);
    assert(customos_history_store_capabilities() == 65UL);
    assert(customos_history_store_self_check() == 0);
    assert(customos_history_store_init() == 0);
    assert(customos_history_store_self_check() == 1);
    return 0;
}
