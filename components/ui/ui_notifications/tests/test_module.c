#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_ui_notifications_name(), "ui_notifications") == 0);
    assert(customos_ui_notifications_capabilities() == 88UL);
    assert(customos_ui_notifications_self_check() == 0);
    assert(customos_ui_notifications_init() == 0);
    assert(customos_ui_notifications_self_check() == 1);
    return 0;
}
