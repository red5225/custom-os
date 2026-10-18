#include "module.h"

static int initialized;

int customos_image_layout_init(void) {
    initialized = 1;
    return 0;
}

unsigned long customos_image_layout_capabilities(void) {
    return 4UL;
}

const char *customos_image_layout_name(void) {
    return "image_layout";
}

int customos_image_layout_self_check(void) {
    return initialized == 1 && customos_image_layout_capabilities() == 4UL;
}
