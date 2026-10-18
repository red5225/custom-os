#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_ui_accessibility_name(), "ui_accessibility") == 0);
    assert(customos_ui_accessibility_capabilities() == 86UL);
    assert(customos_ui_accessibility_self_check() == 0);
    assert(customos_ui_accessibility_init() == 0);
    assert(customos_ui_accessibility_self_check() == 1);
    return 0;
}
