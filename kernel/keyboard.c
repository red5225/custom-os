#include "keyboard.h"
#include "kernel.h"
#include "fb.h"
#include <stdint.h>
#define ST 0x64
#define DATA 0x60
static char input[96]; static int input_len; static uint8_t shift_down,caps_lock;
static char lines[34][96]; static int line_count; static uint32_t cursor_blink;
static const char normal[128]={0,27,49,50,51,52,53,54,55,56,57,48,45,61,8,9,113,119,101,114,116,121,117,105,111,112,91,93,10,0,97,115,100,102,103,104,106,107,108,59,39,96,0,92,122,120,99,118,98,110,109,44,46,47,0,42,0,32,48};
static int same(const char*a,const char*b){int i=0;while(a[i]&&a[i]==b[i])i++;return a[i]==0&&b[i]==0;}
static char shifted(uint8_t s){switch(s){case 2:return 33;case 3:return 64;case 4:return 35;case 5:return 36;case 6:return 37;case 7:return 94;case 8:return 38;case 9:return 42;case 10:return 40;case 11:return 41;case 12:return 95;case 13:return 43;case 26:return 123;case 27:return 125;case 39:return 58;case 40:return 34;case 41:return 126;case 43:return 124;case 51:return 60;case 52:return 62;case 53:return 63;default:return normal[s];}}
static char key_char(uint8_t s){if(s>=128)return 0;char c=normal[s];if(!c)return 0;if(c>=97&&c<=122)return(shift_down^caps_lock)?(char)(c-32):c;return shift_down?shifted(s):c;}
static void clear_lines(void){line_count=0;for(int i=0;i<34;i++)lines[i][0]=0;}
static void add_line(const char*s){if(line_count>=34){for(int i=1;i<34;i++)for(int j=0;j<96;j++)lines[i-1][j]=lines[i][j];line_count=33;}int i=0;while(s[i]&&i<95){lines[line_count][i]=s[i];i++;}lines[line_count][i]=0;line_count++;}
static void append_output(const char*s){char l[96];int j=0;while(*s){if(*s==10||j>=95){l[j]=0;add_line(l);j=0;if(*s==10){s++;continue;}}l[j++]=*s++;}if(j){l[j]=0;add_line(l);}}
static void render(void){kernel_draw_terminal();uint32_t W=fb_width(),H=fb_height(),tw=W>900?760:(W>700?W-120:W-48),th=H>650?500:(H>500?H-150:H-100),tx=(W-tw)/2,ty=(H-th)/2,bg=0xFF07090D,fg=0xFFE5EAF2,accent=0xFF67A1FF,x=tx+24,y=ty+58;int rows=(int)((th-92)/16);if(rows>34)rows=34;int start=line_count-rows;if(start<0)start=0;for(int i=start;i<line_count;i++){fb_draw_string(x,y,lines[i],fg,bg);y+=16;}uint32_t py=ty+th-52;fb_rect(tx+16,py-8,tw-32,1,0xFF202733);fb_draw_string(tx+24,py,">",accent,bg);fb_draw_string(tx+40,py,input,fg,bg);fb_rect(tx+40+(uint32_t)input_len*8,py,8,14,accent);}
static void execute(void){input[input_len]=0;if(!input_len){render();return;}if(same(input,"help"))append_output("Commands: help, clear, echo <text>, time, reboot");else if(same(input,"clear"))clear_lines();else if(same(input,"time"))append_output("Hardware input polling active.");else if(same(input,"reboot")){append_output("Rebooting...");render();while(port_byte_in(ST)&2){}port_byte_out(ST,0xFE);for(;;);}else if(input_len>=5&&input[0]=='e'&&input[1]=='c'&&input[2]=='h'&&input[3]=='o'&&input[4]==' ')append_output(input+5);else append_output("Command not found. Type 'help'.");input_len=0;input[0]=0;render();}
static void consume(uint8_t s){if(s==0xE0||s==0xE1)return;if(s==0x2A||s==0x36){shift_down=1;return;}if(s==0xAA||s==0xB6){shift_down=0;return;}if(s==0x3A){caps_lock=!caps_lock;return;}if(s&0x80)return;if(s==0x1C){execute();return;}if(s==0x0E){if(input_len){input[--input_len]=0;render();}return;}char c=key_char(s);if(c&&input_len<95){input[input_len++]=c;input[input_len]=0;render();}}
void keyboard_init(void){input_len=0;shift_down=0;caps_lock=0;clear_lines();append_output("CUSTOM OS");append_output("v1.0 | PS/2 input");append_output("Keyboard polling active");append_output("Type 'help' for commands.");render();}
void keyboard_poll(void){for(int n=0;n<16;n++){uint8_t s=port_byte_in(ST);if(!(s&1))break;uint8_t d=port_byte_in(DATA);if(s&0x20)continue;consume(d);}}
void keyboard_isr_handler(void){keyboard_poll();port_byte_out(0x20,0x20);}
void keyboard_on_timer_tick(void){cursor_blink++;}