#ifndef KERNEL_H
#define KERNEL_H
#include <stdint.h>
void kernel_main(void);
void kernel_draw_terminal(void);
void timer_isr_handler(void);
void keyboard_isr_handler(void);
void mouse_isr_handler(void);
void keyboard_on_timer_tick(void);
uint8_t port_byte_in(uint16_t port);
void port_byte_out(uint16_t port,uint8_t data);
extern unsigned int mb2_info_addr;
extern int g_fb_mode;
extern int g_gui_enabled;
#endif
