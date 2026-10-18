#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_line_editor_name(), "line_editor") == 0);
    assert(customos_line_editor_capabilities() == 62UL);
    assert(customos_line_editor_self_check() == 0);
    assert(customos_line_editor_init() == 0);
    assert(customos_line_editor_self_check() == 1);
    return 0;
}
