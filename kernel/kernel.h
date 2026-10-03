#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

void kernel_main(void);
void timer_isr_handler(void);
void keyboard_isr_handler(void);
void mouse_isr_handler(void);

// I/O functions
uint8_t port_byte_in(uint16_t port);
void port_byte_out(uint16_t port, uint8_t data);

// Multiboot2 info pointer saved by boot.s
extern unsigned int mb2_info_addr;

// Video mode selection (set by parsing GRUB cmdline)
extern int g_fb_mode; // 1 = framebuffer mode, 0 = text mode
extern int g_gui_enabled; // gate for GUI tick

#endif