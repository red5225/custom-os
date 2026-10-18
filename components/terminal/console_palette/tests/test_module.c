#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_console_palette_name(), "console_palette") == 0);
    assert(customos_console_palette_capabilities() == 66UL);
    assert(customos_console_palette_self_check() == 0);
    assert(customos_console_palette_init() == 0);
    assert(customos_console_palette_self_check() == 1);
    return 0;
}
