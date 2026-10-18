#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_prompt_builder_name(), "prompt_builder") == 0);
    assert(customos_prompt_builder_capabilities() == 69UL);
    assert(customos_prompt_builder_self_check() == 0);
    assert(customos_prompt_builder_init() == 0);
    assert(customos_prompt_builder_self_check() == 1);
    return 0;
}
