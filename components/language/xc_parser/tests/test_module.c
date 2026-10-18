#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_xc_parser_name(), "xc_parser") == 0);
    assert(customos_xc_parser_capabilities() == 72UL);
    assert(customos_xc_parser_self_check() == 0);
    assert(customos_xc_parser_init() == 0);
    assert(customos_xc_parser_self_check() == 1);
    return 0;
}
