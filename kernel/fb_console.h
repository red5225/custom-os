#ifndef FB_CONSOLE_H
#define FB_CONSOLE_H

#include <stdint.h>

void fbcon_init(uint32_t fg, uint32_t bg);
void fbcon_clear(void);
void fbcon_putc(char c);
void fbcon_write(const char* s);

// Draw a simple welcome screen in framebuffer, similar to text mode
void fb_show_welcome_screen(void);

#endif
