#ifndef TERMINAL_H
#define TERMINAL_H
struct limine_framebuffer;
void terminal_init(void);
void terminal_init_fb(const struct limine_framebuffer *fb);
void terminal_clear(void);
void terminal_putc(char c);
void terminal_write(const char *s);
void terminal_backspace(void);
#endif
