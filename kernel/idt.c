#include "idt.h"
#include "kernel.h"
#include "keyboard.h"
#include <stdint.h>
extern void isr_stub_irq0(void);extern void isr_stub_irq1(void);extern void isr_stub_irq12(void);
static volatile uint32_t ticks=0;
uint32_t timer_ticks(void){return ticks;}
void timer_isr_handler(void){ticks++;keyboard_on_timer_tick();port_byte_out(0x20,0x20);}
void pit_init(uint32_t freq){uint32_t divisor=1193180/(freq?freq:100);port_byte_out(0x43,0x34);port_byte_out(0x40,(uint8_t)divisor);port_byte_out(0x40,(uint8_t)(divisor>>8));uint8_t mask=port_byte_in(0x21);mask&=~1u;port_byte_out(0x21,mask);}
