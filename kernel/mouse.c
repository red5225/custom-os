#include "mouse.h"

static inline void outb(unsigned short p,unsigned char v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));}
static inline unsigned char inb(unsigned short p){unsigned char v;__asm__ volatile("inb %1,%0":"=a"(v):"Nd"(p));return v;}

static void wait_write(void){unsigned int n=100000;while(n--&&(inb(0x64)&2));}
static void wait_read(void){unsigned int n=100000;while(n--&&!(inb(0x64)&1));}

static void mouse_write(unsigned char v){
	wait_write();outb(0x64,0xD4);
	wait_write();outb(0x60,v);
}
void mouse_init(void){
	wait_write();outb(0x64,0xA8);
	wait_write();outb(0x64,0x20);
	wait_read();
	unsigned char status=inb(0x60);
	status|=2;
	wait_write();outb(0x64,0x60);
	wait_write();outb(0x60,status);
	mouse_write(0xF4);
	wait_read();(void)inb(0x60);
}
int mouse_poll(int *dx,int *dy,int *buttons){
	static unsigned char packet[3];static int n;
	if(!(inb(0x64)&1))return 0;
	unsigned char v=inb(0x60);
	if(!(inb(0x64)&0x20))return 0;
	if(n==0&&!(v&8))return 0;
	packet[n++]=v;
	if(n<3)return 0;
	n=0;
	*dx=(signed char)packet[1];
	*dy=-(signed char)packet[2];
	*buttons=packet[0]&7;
	return 1;
}
