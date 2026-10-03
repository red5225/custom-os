#include "kernel.h"
#include "idt.h"
#include "serial.h"
#include "keyboard.h"
#include "mouse.h"
#include "fb.h"
#include "screen.h"
#include <stdint.h>

int g_fb_mode=1;
int g_gui_enabled=0;

extern const unsigned char _binary_wallpaper_rgb_start[];
extern const unsigned char _binary_wallpaper_rgb_end[];

static uint32_t wallpaper_size(void){return (uint32_t)(_binary_wallpaper_rgb_end-_binary_wallpaper_rgb_start);}
static uint32_t rgb(uint8_t r,uint8_t g,uint8_t b){return 0xFF000000u|((uint32_t)r<<16)|((uint32_t)g<<8)|b;}
static void rect(uint32_t x,uint32_t y,uint32_t w,uint32_t h,uint32_t c){
    if(x>=fb_width()||y>=fb_height())return;
    if(x+w>fb_width())w=fb_width()-x;
    if(y+h>fb_height())h=fb_height()-y;
    fb_rect(x,y,w,h,c);
}
static void text(uint32_t x,uint32_t y,const char*s,uint32_t fg,uint32_t bg){fb_draw_string(x,y,s,fg,bg);}

static void draw_wallpaper(void){
    const uint32_t iw=640,ih=480;
    if(wallpaper_size()<iw*ih*3u){fb_fill(0xFF101722u);return;}
    uint32_t w=fb_width(),h=fb_height();
    for(uint32_t y=0;y<h;y++){
        uint32_t sy=(y*ih)/h;
        for(uint32_t x=0;x<w;x++){
            uint32_t sx=(x*iw)/w;
            const unsigned char*p=_binary_wallpaper_rgb_start+(sy*iw+sx)*3u;
            fb_putpixel(x,y,rgb(p[0],p[1],p[2]));
        }
    }
}

void kernel_draw_terminal(void){
    draw_wallpaper();
    uint32_t w=fb_width(),h=fb_height();
    uint32_t tw=w>900?760:(w>700?w-120:w-48);
    uint32_t th=h>650?500:(h>500?h-150:h-100);
    uint32_t tx=(w-tw)/2,ty=(h-th)/2;
    rect(0,0,w,42,0xF20B1018u);
    rect(0,41,w,1,0xFF344254u);
    rect(18,12,18,18,0xFF5B8CFFu);
    rect(23,16,8,10,0xFF0B1018u);
    text(48,13,"Custom OS",0xFFE9EEF7u,0xFF0B1018u);
    text(w>170?w-150:20,13,"Terminal",0xFF9AA8BBu,0xFF0B1018u);
    rect(tx+4,ty+4,tw,th,0x55000000u);
    rect(tx,ty,tw,th,0xFF0A0D12u);
    rect(tx,ty,tw,38,0xFF171D26u);
    rect(tx,ty+37,tw,1,0xFF303A49u);
    rect(tx+15,ty+14,9,9,0xFFEF6B73u);
    rect(tx+31,ty+14,9,9,0xFFE5B85Cu);
    rect(tx+47,ty+14,9,9,0xFF61C08Bu);
    text(tx+76,ty+12,"Terminal",0xFFE6EBF3u,0xFF171D26u);
    rect(tx+1,ty+39,tw-2,th-40,0xFF07090Du);
    rect(tx+16,ty+th-30,tw-32,1,0xFF202733u);
    rect(w/2-54,h-56,108,38,0xCC0A0D12u);
    rect(w/2-40,h-48,18,18,0xFF5B8CFFu);
    text(w/2-14,h-49,"Terminal",0xFFD8E0ECu,0xFF0A0D12u);
}

void kernel_main(void){
    serial_init();
    idt_init();
    pit_init(100);
    g_fb_mode=1;
    if(!fb_init_from_multiboot(mb2_info_addr)||!fb_is_available()){
        g_fb_mode=0;
        clear_screen_color(VGA_COLOR_BLACK,VGA_COLOR_BLACK);
        print_string("CUSTOM OS\n",vga_entry_color(VGA_COLOR_LIGHT_CYAN,VGA_COLOR_BLACK),2,2);
        print_string("Framebuffer unavailable. Text mode active.\n",vga_entry_color(VGA_COLOR_LIGHT_GRAY,VGA_COLOR_BLACK),2,4);
    }else{
        kernel_draw_terminal();
    }
    keyboard_init();
    mouse_init();
    __asm__ __volatile__("sti");
    for(;;){
        if(g_fb_mode){int mx,my;uint8_t buttons;mouse_get(&mx,&my,&buttons);(void)mx;(void)my;(void)buttons;}
        __asm__ __volatile__("hlt");
    }
}
void port_byte_out(uint16_t port,uint8_t data){__asm__ __volatile__("outb %0,%1"::"a"(data),"dN"(port));}
uint8_t port_byte_in(uint16_t port){uint8_t v;__asm__ __volatile__("inb %1,%0":"=a"(v):"dN"(port));return v;}
