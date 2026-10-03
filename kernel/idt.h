#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_init(void);
void pit_init(uint32_t freq);
uint32_t timer_ticks(void);

#endif