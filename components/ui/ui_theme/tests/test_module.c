#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_ui_theme_name(), "ui_theme") == 0);
    assert(customos_ui_theme_capabilities() == 81UL);
    assert(customos_ui_theme_self_check() == 0);
    assert(customos_ui_theme_init() == 0);
    assert(customos_ui_theme_self_check() == 1);
    return 0;
}
