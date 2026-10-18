#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_image_layout_name(), "image_layout") == 0);
    assert(customos_image_layout_capabilities() == 4UL);
    assert(customos_image_layout_self_check() == 0);
    assert(customos_image_layout_init() == 0);
    assert(customos_image_layout_self_check() == 1);
    return 0;
}
