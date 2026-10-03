#include "idt.h"
#include "kernel.h"
#include "keyboard.h"
#include <stdint.h>

typedef struct __attribute__((packed)){uint16_t off_lo,sel;uint8_t zero,flags;uint16_t off_hi;} idt_gate_t;
typedef struct __attribute__((packed)){uint16_t limit;uint32_t base;} idtr_t;

static idt_gate_t idt[256];

extern void isr_stub_irq0(void);
extern void isr_stub_irq1(void);
extern void isr_stub_irq12(void);
extern void isr0(void);
extern void isr1(void);

static void set_gate(int n,void(*fn)(void),uint8_t flags){
    uint32_t a=(uint32_t)fn;
    idt[n].off_lo=(uint16_t)a;
    idt[n].sel=0x08;
    idt[n].zero=0;
    idt[n].flags=flags;
    idt[n].off_hi=(uint16_t)(a>>16);
}
static void lidt(const idtr_t*p){__asm__ __volatile__("lidtl (%0)"::"r"(p));}

void idt_init(void){
    for(int i=0;i<256;i++) set_gate(i,isr0,0x8E);
    set_gate(32,isr_stub_irq0,0x8E);
    set_gate(33,isr_stub_irq1,0x8E);
    set_gate(44,isr_stub_irq12,0x8E);

    /* Remap the legacy PIC to vectors 32-47. */
    port_byte_out(0x20,0x11); port_byte_out(0xA0,0x11);
    port_byte_out(0x21,0x20); port_byte_out(0xA1,0x28);
    port_byte_out(0x21,0x04); port_byte_out(0xA1,0x02);
    port_byte_out(0x21,0x01); port_byte_out(0xA1,0x01);

    /* Timer + keyboard enabled; mouse IRQ stays masked because mouse is polled. */
    port_byte_out(0x21,0xFC);
    port_byte_out(0xA1,0xFF);

    idtr_t r={(uint16_t)(sizeof(idt)-1),(uint32_t)idt};
    lidt(&r);
}
void fault_isr_handler(void){
    port_byte_out(0x20,0x20);
    for(;;)__asm__ __volatile__("hlt");
}
void timer_isr_handler(void);
void pit_init(uint32_t freq){
    uint32_t divisor=1193180/(freq?freq:100);
    port_byte_out(0x43,0x34);
    port_byte_out(0x40,(uint8_t)divisor);
    port_byte_out(0x40,(uint8_t)(divisor>>8));
}
uint32_t timer_ticks(void){static uint32_t t;return t;}
