#include "keyboard.h"
#include "kernel.h"
#include <stdint.h>
#define STATUS 0x64
#define DATA 0x60
static char input[64]; static int len; static uint8_t shift;
static const char map[59]={'\0',27,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p','[',']','\n','\0','a','s','d','f','g','h','j','k','l',';','\'', '`','\0','\\','z','x','c','v','b','n','m',',','.','/','\0','*','\0',' ','\0'};
static char upper(uint8_t s){char c=map[s];if(c>='a'&&c<='z')c=(char)(c-32);switch(s){case 2:return '!';case 3:return '@';case 4:return '#';case 5:return '$';case 6:return '%';case 7:return '^';case 8:return '&';case 9:return '*';case 10:return '(';case 11:return ')';case 12:return '_';case 13:return '+';default:return c;}}
static void redraw(void){print_string(input,vga_entry_color(VGA_COLOR_WHITE,VGA_COLOR_BLACK),4,10);}
static void run_cmd(void){input[len]=0;print_string("\n",vga_entry_color(VGA_COLOR_WHITE,VGA_COLOR_BLACK),2,12);if(len==4&&input[0]=='h'&&input[1]=='e'&&input[2]=='l'&&input[3]=='p')print_string("help  clear  echo  reboot",vga_entry_color(VGA_COLOR_LIGHT_GRAY,VGA_COLOR_BLACK),4,13);else if(len==5&&input[0]=='c'&&input[1]=='l'&&input[2]=='e'&&input[3]=='a'&&input[4]=='r')clear_screen_color(VGA_COLOR_BLACK,VGA_COLOR_BLACK);else if(len>=5&&input[0]=='e'&&input[1]=='c'&&input[2]=='h'&&input[3]=='o'&&input[4]==' ')print_string(input+5,vga_entry_color(VGA_COLOR_LIGHT_GRAY,VGA_COLOR_BLACK),4,13);else print_string("Unknown command",vga_entry_color(VGA_COLOR_LIGHT_GRAY,VGA_COLOR_BLACK),4,13);len=0;input[0]=0;print_string("> ",vga_entry_color(VGA_COLOR_WHITE,VGA_COLOR_BLACK),2,15);}
void keyboard_init(void){len=0;shift=0;input[0]=0;}
void keyboard_poll(void){for(int n=0;n<8;n++){uint8_t s=port_byte_in(STATUS);if(!(s&1))return;uint8_t k=port_byte_in(DATA);if(s&0x20)continue;if(k==0x2A||k==0x36){shift=1;continue;}if(k==0xAA||k==0xB6){shift=0;continue;}if(k&0x80)continue;if(k==0x1C){run_cmd();continue;}if(k==0x0E){if(len){len--;input[len]=0;redraw();}continue;}if(k<59){char c=shift?upper(k):map[k];if(c&&len<62){input[len++]=c;input[len]=0;redraw();}}}}
void keyboard_isr_handler(void){keyboard_poll();port_byte_out(0x20,0x20);}
void keyboard_on_timer_tick(void){}
