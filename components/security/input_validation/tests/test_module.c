#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_input_validation_name(), "input_validation") == 0);
    assert(customos_input_validation_capabilities() == 97UL);
    assert(customos_input_validation_self_check() == 0);
    assert(customos_input_validation_init() == 0);
    assert(customos_input_validation_self_check() == 1);
    return 0;
}
