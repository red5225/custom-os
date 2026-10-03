#include "mouse.h"
#include "kernel.h"
#include "fb.h"
#include <stdint.h>
static int mx=160,my=120; static uint8_t buttons,packet[3],pos,ready;
static int wr(void){for(uint32_t i=0;i<100000;i++)if(!(port_byte_in(0x64)&2))return 1;return 0;}
static int rd(void){for(uint32_t i=0;i<100000;i++)if(port_byte_in(0x64)&1)return 1;return 0;}
static void flush(void){for(int i=0;i<32;i++){if(!(port_byte_in(0x64)&1))break;(void)port_byte_in(0x60);}}
static int cmd(uint8_t v){if(!wr())return 0;port_byte_out(0x64,0xD4);if(!wr())return 0;port_byte_out(0x60,v);return rd();}
void mouse_init(void){ready=0;pos=0;flush();if(!wr())return;port_byte_out(0x64,0xA8);if(!wr())return;port_byte_out(0x64,0x20);if(!rd())return;uint8_t c=port_byte_in(0x60);c|=2;c&=(uint8_t)~0x20;if(!wr())return;port_byte_out(0x64,0x60);if(!wr())return;port_byte_out(0x60,c);flush();if(!cmd(0xF6))return;(void)port_byte_in(0x60);if(!cmd(0xF4))return;(void)port_byte_in(0x60);ready=1;}
static void packet_done(void){uint8_t b=packet[0];if(!(b&8))return;int dx=(int8_t)packet[1],dy=(int8_t)packet[2];if(b&0x40)dx=0;if(b&0x80)dy=0;mx+=dx;my-=dy;int maxx=(int)fb_width()-8,maxy=(int)fb_height()-8;if(mx<0)mx=0;if(my<0)my=0;if(mx>maxx)mx=maxx;if(my>maxy)my=maxy;buttons=b&7;}
void mouse_poll(void){if(!ready)return;for(int n=0;n<16;n++){uint8_t s=port_byte_in(0x64);if(!(s&1)||!(s&0x20))break;uint8_t b=port_byte_in(0x60);if(pos==0&&!(b&8))continue;packet[pos++]=b;if(pos==3){pos=0;packet_done();}}}
void mouse_isr_handler(void){mouse_poll();port_byte_out(0xA0,0x20);port_byte_out(0x20,0x20);}
void mouse_get(int*x,int*y,uint8_t*b){mouse_poll();if(x)*x=mx;if(y)*y=my;if(b)*b=buttons;}