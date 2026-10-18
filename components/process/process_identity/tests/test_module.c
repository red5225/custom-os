#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_process_identity_name(), "process_identity") == 0);
    assert(customos_process_identity_capabilities() == 39UL);
    assert(customos_process_identity_self_check() == 0);
    assert(customos_process_identity_init() == 0);
    assert(customos_process_identity_self_check() == 1);
    return 0;
}
