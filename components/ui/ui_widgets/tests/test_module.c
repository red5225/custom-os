#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_ui_widgets_name(), "ui_widgets") == 0);
    assert(customos_ui_widgets_capabilities() == 83UL);
    assert(customos_ui_widgets_self_check() == 0);
    assert(customos_ui_widgets_init() == 0);
    assert(customos_ui_widgets_self_check() == 1);
    return 0;
}
