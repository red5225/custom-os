#include "mouse.h"

static uint8_t inb(uint16_t port) { uint8_t v; __asm__ volatile("inb %1,%0" : "=a"(v) : "Nd"(port)); return v; }
static void outb(uint16_t port, uint8_t v) { __asm__ volatile("outb %0,%1" :: "a"(v), "Nd"(port)); }
static int wait_input_clear(void) { for (uint32_t i=0;i<100000u;i++) if (!(inb(0x64)&2u)) return 1; return 0; }
static int wait_aux_output(void) { for (uint32_t i=0;i<100000u;i++) { uint8_t s=inb(0x64); if ((s&0x21u)==0x21u) return 1; } return 0; }
static int send_aux(uint8_t value) {
    if (!wait_input_clear()) return 0; outb(0x64,0xD4);
    if (!wait_input_clear()) return 0; outb(0x60,value);
    if (!wait_aux_output()) return 0; return inb(0x60)==0xFA;
}
void mouse_init(void) {
    if (wait_input_clear()) outb(0x64,0xA8); /* enable auxiliary port */
    (void)send_aux(0xF6); /* defaults */
    (void)send_aux(0xF4); /* stream mode */
}
int mouse_poll(mouse_event_t *event) {
    if (!event) return 0;
    uint8_t status=inb(0x64);
    if ((status&0x21u)!=0x21u) return 0;
    uint8_t first=inb(0x60);
    if (!(first&0x08u)) return 0;
    if (!wait_aux_output()) return 0; uint8_t x=inb(0x60);
    if (!wait_aux_output()) return 0; uint8_t y=inb(0x60);
    event->buttons=(uint8_t)(first&0x07u);
    event->dx=(first&0x40u)?0:(int8_t)x;
    event->dy=(first&0x80u)?0:(int16_t)-(int8_t)y;
    return 1;
}
