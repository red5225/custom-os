#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_ui_status_panel_name(), "ui_status_panel") == 0);
    assert(customos_ui_status_panel_capabilities() == 90UL);
    assert(customos_ui_status_panel_self_check() == 0);
    assert(customos_ui_status_panel_init() == 0);
    assert(customos_ui_status_panel_self_check() == 1);
    return 0;
}
