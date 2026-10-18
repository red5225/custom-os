#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_ui_settings_name(), "ui_settings") == 0);
    assert(customos_ui_settings_capabilities() == 87UL);
    assert(customos_ui_settings_self_check() == 0);
    assert(customos_ui_settings_init() == 0);
    assert(customos_ui_settings_self_check() == 1);
    return 0;
}
