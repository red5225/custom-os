#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>
void keyboard_init(void);
void keyboard_poll(void);
void keyboard_isr_handler(void);
void keyboard_on_timer_tick(void);
#endif
