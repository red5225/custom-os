#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_ui_focus_name(), "ui_focus") == 0);
    assert(customos_ui_focus_capabilities() == 84UL);
    assert(customos_ui_focus_self_check() == 0);
    assert(customos_ui_focus_init() == 0);
    assert(customos_ui_focus_self_check() == 1);
    return 0;
}
