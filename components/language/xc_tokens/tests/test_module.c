#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_xc_tokens_name(), "xc_tokens") == 0);
    assert(customos_xc_tokens_capabilities() == 71UL);
    assert(customos_xc_tokens_self_check() == 0);
    assert(customos_xc_tokens_init() == 0);
    assert(customos_xc_tokens_self_check() == 1);
    return 0;
}
