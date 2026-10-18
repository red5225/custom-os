#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_ui_events_name(), "ui_events") == 0);
    assert(customos_ui_events_capabilities() == 85UL);
    assert(customos_ui_events_self_check() == 0);
    assert(customos_ui_events_init() == 0);
    assert(customos_ui_events_self_check() == 1);
    return 0;
}
