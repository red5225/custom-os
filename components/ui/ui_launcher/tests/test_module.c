#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_ui_launcher_name(), "ui_launcher") == 0);
    assert(customos_ui_launcher_capabilities() == 89UL);
    assert(customos_ui_launcher_self_check() == 0);
    assert(customos_ui_launcher_init() == 0);
    assert(customos_ui_launcher_self_check() == 1);
    return 0;
}
