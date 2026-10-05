#include "mouse.h"

static inline void outb(unsigned short p,unsigned char v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));}
static inline unsigned char inb(unsigned short p){unsigned char v;__asm__ volatile("inb %1,%0":"=a"(v):"Nd"(p));return v;}
static void wait_write(void){unsigned int n=100000;while(n--&&(inb(0x64)&2));}
static void wait_read(void){unsigned int n=100000;while(n--&&!(inb(0x64)&1));}
static void aux(unsigned char v){wait_write();outb(0x64,0xD4);wait_write();outb(0x60,v);}
void mouse_init(void){
    wait_write();outb(0x64,0xA8);
    wait_write();outb(0x64,0x20);
    unsigned char s=inb(0x60);
    s|=2;
    wait_write();outb(0x64,0x60);
    wait_write();outb(0x60,s);
    aux(0xF4);wait_read();(void)inb(0x60);
}
int mouse_poll(int *dx,int *dy,int *buttons){
    static unsigned char p[3];static int n=0;
    unsigned char s=inb(0x64);
    if(!(s&1)||!(s&0x20))return 0;
    unsigned char v=inb(0x60);
    if(n==0&&!(v&8))return 0;
    p[n++]=v;
    if(n<3)return 0;
    n=0;
    *dx=(signed char)p[1];*dy=-(signed char)p[2];*buttons=p[0]&7;
    return 1;
}
