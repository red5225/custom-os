#include "mouse.h"
#include "kernel.h"
#include "fb.h"
#include <stdint.h>
static int mx=120,my=120;static uint8_t buttons=0,packet[3],packet_pos=0,mouse_ready=0;
static int wait_write(void){for(uint32_t i=0;i<200000;i++)if(!(port_byte_in(0x64)&2))return 1;return 0;}
static int wait_read(void){for(uint32_t i=0;i<200000;i++)if(port_byte_in(0x64)&1)return 1;return 0;}
static int send(uint8_t v){if(!wait_write())return 0;port_byte_out(0x64,0xD4);if(!wait_write())return 0;port_byte_out(0x60,v);return 1;}
static void flush(void){for(int i=0;i<32;i++){if(!(port_byte_in(0x64)&1))break;(void)port_byte_in(0x60);}}
void mouse_init(void){
    flush();if(!wait_write())return;port_byte_out(0x64,0xA8);
    if(!wait_write())return;port_byte_out(0x64,0x20);if(!wait_read())return;
    uint8_t cmd=port_byte_in(0x60);cmd&=(uint8_t)~0x02;cmd&=(uint8_t)~0x20;
    if(!wait_write())return;port_byte_out(0x64,0x60);if(!wait_write())return;port_byte_out(0x60,cmd);flush();
    if(send(0xF6)&&wait_read())(void)port_byte_in(0x60);if(send(0xF4)&&wait_read())(void)port_byte_in(0x60);
    packet_pos=0;mouse_ready=1;
}
static void process_packet(void){
    uint8_t b0=packet[0];if(!(b0&8))return;int dx=(int8_t)packet[1],dy=(int8_t)packet[2];
    if(b0&0x40)dx=0;if(b0&0x80)dy=0;mx+=dx;my-=dy;
    int maxx=(int)fb_width()-10,maxy=(int)fb_height()-10;if(mx<0)mx=0;if(my<0)my=0;if(mx>maxx)mx=maxx;if(my>maxy)my=maxy;buttons=b0&7;
}
void mouse_poll(void){
    if(!mouse_ready)return;
    for(int n=0;n<8;n++){uint8_t s=port_byte_in(0x64);if(!(s&1)||!(s&0x20))break;uint8_t b=port_byte_in(0x60);if(packet_pos==0&&!(b&8))continue;packet[packet_pos++]=b;if(packet_pos==3){packet_pos=0;process_packet();}}
}
void mouse_isr_handler(void){(void)port_byte_in(0x60);port_byte_out(0xA0,0x20);port_byte_out(0x20,0x20);}
void mouse_get(int*x,int*y,uint8_t*b){mouse_poll();if(x)*x=mx;if(y)*y=my;if(b)*b=buttons;}
