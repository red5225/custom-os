#include "kernel.h"
#include "keyboard.h"
#include "mouse.h"
#include "fb.h"
#include "screen.h"
#include <stdint.h>
int g_fb_mode=1; int g_gui_enabled=0;
extern unsigned int mb2_info_addr;
extern const unsigned char _binary_wallpaper_rgb_start[];
extern const unsigned char _binary_wallpaper_rgb_end[];
static uint32_t wallpaper_size(void){return(uint32_t)(_binary_wallpaper_rgb_end-_binary_wallpaper_rgb_start);}
static uint32_t rgb(uint8_t r,uint8_t g,uint8_t b){return 0xFF000000u|((uint32_t)r<<16)|((uint32_t)g<<8)|b;}
static void draw_wallpaper(void){
 const uint32_t iw=640,ih=480;if(wallpaper_size()<iw*ih*3u){fb_fill(0xFF101722u);return;}
 uint32_t w=fb_width(),h=fb_height();
 for(uint32_t y=0;y<h;y++){uint32_t sy=(y*ih)/h;for(uint32_t x=0;x<w;x++){uint32_t sx=(x*iw)/w;const unsigned char*p=_binary_wallpaper_rgb_start+(sy*iw+sx)*3u;fb_putpixel(x,y,rgb(p[0],p[1],p[2]));}}
}
static void rect(uint32_t x,uint32_t y,uint32_t w,uint32_t h,uint32_t c){if(x>=fb_width()||y>=fb_height())return;if(x+w>fb_width())w=fb_width()-x;if(y+h>fb_height())h=fb_height()-y;fb_rect(x,y,w,h,c);}
static void text(uint32_t x,uint32_t y,const char*s,uint32_t fg,uint32_t bg){fb_draw_string(x,y,s,fg,bg);}
void kernel_draw_terminal(void){
 draw_wallpaper();uint32_t W=fb_width(),H=fb_height(),tw=W>900?760:(W>700?W-120:W-48),th=H>650?500:(H>500?H-150:H-100),tx=(W-tw)/2,ty=(H-th)/2;
 rect(0,0,W,42,0xF0202A38u);rect(0,41,W,1,0xFF344254u);rect(18,12,18,18,0xFF5B8CFFu);rect(23,16,8,10,0xFF0A0F17u);text(48,13,"Custom OS",0xFFE9EEF7u,0xF0202A38u);text(W>700?W-150:W-105,13,"Terminal",0xFFB9C7D9u,0xF0202A38u);
 rect(tx,ty,tw,th,0xF507090Du);rect(tx,ty,tw,38,0xFF171D27u);rect(tx,ty+37,tw,1,0xFF303A49u);rect(tx+14,ty+14,9,9,0xFFEF6B73u);rect(tx+30,ty+14,9,9,0xFFE5B85Cu);rect(tx+46,ty+14,9,9,0xFF61C454u);text(tx+76,ty+12,"Terminal",0xFFE6EBF3u,0xFF171D27u);
}
void kernel_main(void){
 serial_init();g_fb_mode=1;
 if(!fb_init_from_multiboot(mb2_info_addr)||!fb_is_available()){g_fb_mode=0;clear_screen_color(VGA_COLOR_BLACK,VGA_COLOR_BLACK);print_string("CUSTOM OS\n",vga_entry_color(VGA_COLOR_LIGHT_CYAN,VGA_COLOR_BLACK),2,2);print_string("Framebuffer unavailable. Text mode active.\n",vga_entry_color(VGA_COLOR_LIGHT_GREY,VGA_COLOR_BLACK),2,4);keyboard_init();for(;;){keyboard_poll();}}
 kernel_draw_terminal();keyboard_init();mouse_init();
 /* Deliberately keep CPU interrupts disabled. UTM input is polled, preventing IRQ/triple-fault boot loops. */
 __asm__ __volatile__("cli");
 for(;;){keyboard_poll();mouse_poll();__asm__ __volatile__("pause");}
}
uint8_t port_byte_in(uint16_t port){uint8_t v;__asm__ __volatile__("inb %1,%0":"=a"(v):"Nd"(port));return v;}
void port_byte_out(uint16_t port,uint8_t data){__asm__ __volatile__("outb %0,%1"::"a"(data),"Nd"(port));}
