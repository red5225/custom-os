#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_startup_sequence_name(), "startup_sequence") == 0);
    assert(customos_startup_sequence_capabilities() == 10UL);
    assert(customos_startup_sequence_self_check() == 0);
    assert(customos_startup_sequence_init() == 0);
    assert(customos_startup_sequence_self_check() == 1);
    return 0;
}
