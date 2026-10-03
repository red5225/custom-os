#include "keyboard.h"
#include "kernel.h"
#include <stdint.h>
#define ST 0x64
#define DATA 0x60
static char input[96];static int input_len;static uint8_t shift_down,caps_lock;
static const char normal[128]={0,27,49,50,51,52,53,54,55,56,57,48,45,61,8,9,113,119,101,114,116,121,117,105,111,112,91,93,10,0,97,115,100,102,103,104,106,107,108,59,39,96,0,92,122,120,99,118,98,110,109,44,46,47,0,42,0,32,48};
static char shifted(uint8_t s){switch(s){case 2:return 33;case 3:return 64;case 4:return 35;case 5:return 36;case 6:return 37;case 7:return 94;case 8:return 38;case 9:return 42;case 10:return 40;case 11:return 41;case 12:return 95;case 13:return 43;case 26:return 123;case 27:return 125;case 39:return 58;case 40:return 34;case 41:return 126;case 43:return 124;case 51:return 60;case 52:return 62;case 53:return 63;default:return normal[s];}}
static char key_char(uint8_t s){if(s>=128)return 0;char c=normal[s];if(!c)return 0;if(c>=97&&c<=122)return(shift_down^caps_lock)?(char)(c-32):c;return shift_down?shifted(s):c;}
static int same(const char*a,const char*b){int i=0;while(a[i]&&a[i]==b[i])i++;return a[i]==0&&b[i]==0;}
static void execute(void){input[input_len]=0;print_string("\n> ",vga_entry_color(VGA_COLOR_WHITE,VGA_COLOR_BLACK),2,14);if(same(input,"help"))print_string("help clear echo time reboot",vga_entry_color(VGA_COLOR_LIGHT_GRAY,VGA_COLOR_BLACK),4,15);else if(same(input,"clear"))clear_screen_color(VGA_COLOR_BLACK,VGA_COLOR_BLACK);else if(same(input,"time"))print_string("Hardware polling active.",vga_entry_color(VGA_COLOR_LIGHT_GRAY,VGA_COLOR_BLACK),4,15);else if(input_len>=5&&input[0]=='e'&&input[1]=='c'&&input[2]=='h'&&input[3]=='o'&&input[4]==' ')print_string(input+5,vga_entry_color(VGA_COLOR_LIGHT_GRAY,VGA_COLOR_BLACK),4,15);else if(same(input,"reboot")){port_byte_out(0x64,0xFE);for(;;);}else print_string("Command not found. Type help.",vga_entry_color(VGA_COLOR_LIGHT_GRAY,VGA_COLOR_BLACK),4,15);input_len=0;input[0]=0;}
void keyboard_init(void){input_len=0;shift_down=0;caps_lock=0;}
void keyboard_poll(void){for(int n=0;n<16;n++){uint8_t s=port_byte_in(ST);if(!(s&1))break;uint8_t d=port_byte_in(DATA);if(s&0x20)continue;if(d==0x2A||d==0x36){shift_down=1;continue;}if(d==0xAA||d==0xB6){shift_down=0;continue;}if(d==0x3A){caps_lock=!caps_lock;continue;}if(d&0x80)continue;if(d==0x1C){execute();continue;}if(d==0x0E){if(input_len)input[--input_len]=0;continue;}char c=key_char(d);if(c&&input_len<90){input[input_len++]=c;input[input_len]=0;print_string(input,vga_entry_color(VGA_COLOR_WHITE,VGA_COLOR_BLACK),4,13);}}}
void keyboard_isr_handler(void){keyboard_poll();port_byte_out(0x20,0x20);} void keyboard_on_timer_tick(void){}
