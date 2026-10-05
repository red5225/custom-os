#include "mouse.h"
static inline void outb(unsigned short p,unsigned char v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));}
static inline unsigned char inb(unsigned short p){unsigned char v;__asm__ volatile("inb %1,%0":"=a"(v):"Nd"(p));return v;}
static void ww(void){unsigned int n=100000;while(n--&&(inb(0x64)&2));}
static void wr(void){unsigned int n=100000;while(n--&&!(inb(0x64)&1));}
static void wa(unsigned char v){ww();outb(0x64,0xD4);ww();outb(0x60,v);}
void mouse_init(void){ww();outb(0x64,0xA8);ww();outb(0x64,0x20);wr();unsigned char s=inb(0x60);s|=2;ww();outb(0x64,0x60);ww();outb(0x60,s);wa(0xF4);wr();(void)inb(0x60);}
int mouse_poll(int*x,int*y,int*b){static unsigned char p[3];static int n=0;if(!(inb(0x64)&1))return 0;unsigned char v=inb(0x60);if(!(inb(0x64)&0x20))return 0;if(n==0&&!(v&8))return 0;p[n++]=v;if(n<3)return 0;n=0;int dx=(signed char)p[1],dy=(signed char)p[2];if(p[0]&0x40)dx=0;if(p[0]&0x80)dy=0;*x=dx;*y=-dy;*b=p[0]&7;return 1;}
