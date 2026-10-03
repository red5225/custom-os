#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void keyboard_init(void);
void keyboard_isr_handler(void);
// Called from timer ISR at 100Hz for key repeat
void keyboard_on_timer_tick(void);

#endif