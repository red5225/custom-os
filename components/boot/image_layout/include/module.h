#ifndef CUSTOMOS_BOOT_IMAGE_LAYOUT_H
#define CUSTOMOS_BOOT_IMAGE_LAYOUT_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_image_layout_init(void);
unsigned long customos_image_layout_capabilities(void);
const char *customos_image_layout_name(void);
int customos_image_layout_self_check(void);

#endif
