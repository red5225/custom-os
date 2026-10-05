#include "mouse.h"

static inline void outb(unsigned short p, unsigned char v){ __asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p)); }
static inline unsigned char inb(unsigned short p){ unsigned char v; __asm__ volatile("inb %1,%0":"=a"(v):"Nd"(p)); return v; }
static void wait_write(void){ unsigned int n=100000; while(n-- && (inb(0x64)&2)); }
static void wait_read(void){ unsigned int n=100000; while(n-- && !(inb(0x64)&1)); }
static void write_aux(unsigned char v){ wait_write(); outb(0x64,0xD4); wait_write(); outb(0x60,v); }

void mouse_init(void){
    wait_write(); outb(0x64,0xA8);
    wait_write(); outb(0x64,0x20);
    wait_read();
    unsigned char status=inb(0x60);
    status|=2;
    wait_write(); outb(0x64,0x60);
    wait_write(); outb(0x60,status);
    write_aux(0xF4);
    wait_read(); (void)inb(0x60);
}

int mouse_poll(int *x,int *y,int *buttons){
    static unsigned char packet[3];
    static int n=0;
    unsigned char status=inb(0x64);
    if(!(status&1) || !(status&0x20)) return 0;
    unsigned char v=inb(0x60);
    if(n==0 && !(v&8)) return 0;
    packet[n++]=v;
    if(n<3) return 0;
    n=0;
    *x=(signed char)packet[1];
    *y=-(signed char)packet[2];
    if(packet[0]&0x40)*x=0;
    if(packet[0]&0x80)*y=0;
    *buttons=packet[0]&7;
    return 1;
}
