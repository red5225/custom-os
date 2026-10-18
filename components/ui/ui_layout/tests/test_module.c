#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_ui_layout_name(), "ui_layout") == 0);
    assert(customos_ui_layout_capabilities() == 82UL);
    assert(customos_ui_layout_self_check() == 0);
    assert(customos_ui_layout_init() == 0);
    assert(customos_ui_layout_self_check() == 1);
    return 0;
}
