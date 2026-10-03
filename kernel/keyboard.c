#include "keyboard.h"
#include "kernel.h"
#include "fb.h"
#include <stdint.h>
#define MAX_INPUT 96
#define MAX_LINES 34
#define MAX_LINE 96
static char input[MAX_INPUT]; static int input_len=0; static uint8_t shift_down=0,caps_lock=0;
static char lines[MAX_LINES][MAX_LINE]; static int line_count=0; static uint32_t cursor_blink=0;
static const char normal[128]={0,27,'1','2','3','4','5','6','7','8','9','0','-','=',8,9,'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v','b','n','m',',','.','/',0,'*',0,' ','0',0,0,0,0,0,0,0};
static int same(const char*a,const char*b){int i=0;while(a[i]&&a[i]==b[i])i++;return a[i]==0&&b[i]==0;}
static char shifted(uint8_t sc){switch(sc){case 2:return '!';case 3:return '@';case 4:return '#';case 5:return '$';case 6:return '%';case 7:return '^';case 8:return '&';case 9:return '*';case 10:return '(';case 11:return ')';case 12:return '_';case 13:return '+';case 26:return '{';case 27:return '}';case 39:return ':';case 40:return '"';case 41:return '~';case 43:return '|';case 51:return '<';case 52:return '>';case 53:return '?';default:return normal[sc];}}
static char key_char(uint8_t sc){if(sc>=128)return 0;char c=normal[sc];if(!c)return 0;if(c>='a'&&c<='z')return(shift_down^caps_lock)?(char)(c-'a'+'A'):c;return shift_down?shifted(sc):c;}
static void clear_lines(void){line_count=0;for(int i=0;i<MAX_LINES;i++)lines[i][0]=0;}
static void add_line(const char*s){if(line_count>=MAX_LINES){for(int i=1;i<MAX_LINES;i++)for(int j=0;j<MAX_LINE;j++)lines[i-1][j]=lines[i][j];line_count=MAX_LINES-1;}int i=0;while(s[i]&&i<MAX_LINE-1){lines[line_count][i]=s[i];i++;}lines[line_count][i]=0;line_count++;}
static void append_output(const char*s){char line[MAX_LINE];int j=0;while(*s){if(*s=='\n'||j>=MAX_LINE-1){line[j]=0;add_line(line);j=0;if(*s=='\n'){s++;continue;}}line[j++]=*s++;}if(j){line[j]=0;add_line(line);}}
static void draw_cursor(uint32_t x,uint32_t y,uint32_t c){if((cursor_blink/50u)&1u)return;fb_rect(x,y,8,14,c);}
static void render(void){
    kernel_draw_terminal();uint32_t W=fb_width(),H=fb_height();uint32_t tw=W>900?760:(W>700?W-120:W-48),th=H>650?500:(H>500?H-150:H-100);uint32_t tx=(W-tw)/2,ty=(H-th)/2,fg=0xFFE5EAF2u,bg=0xFF07090Du,accent=0xFF67A1FFu;uint32_t x=tx+24,y=ty+58;int rows=(int)((th-92)/16);if(rows>MAX_LINES)rows=MAX_LINES;int start=line_count-rows;if(start<0)start=0;for(int i=start;i<line_count;i++){fb_draw_string(x,y,lines[i],fg,bg);y+=16;}uint32_t py=ty+th-52;fb_rect(tx+16,py-8,tw-32,1,0xFF202733u);fb_draw_string(tx+24,py,">",accent,bg);fb_draw_string(tx+40,py,input,fg,bg);draw_cursor(tx+40+(uint32_t)input_len*8,py,accent);
}
static void execute(void){
    input[input_len]=0;if(input_len==0){render();return;}
    if(same(input,"help"))append_output("Commands: help, clear, echo <text>, time, reboot");
    else if(same(input,"clear"))clear_lines();
    else if(same(input,"time"))append_output("System timer is running at 100 Hz.");
    else if(same(input,"reboot")){append_output("Rebooting...");render();while(port_byte_in(0x64)&2){}port_byte_out(0x64,0xFE);for(;;)__asm__ __volatile__("hlt");}
    else if(input_len>=5&&input[0]=='e'&&input[1]=='c'&&input[2]=='h'&&input[3]=='o'&&input[4]==' ')append_output(input+5);
    else append_output("Command not found. Type 'help'.");
    input_len=0;input[0]=0;render();
}
void keyboard_init(void){input_len=0;shift_down=0;caps_lock=0;clear_lines();append_output("CUSTOM OS");append_output("v1.0  |  terminal");append_output("Type 'help' for commands.");render();}
void keyboard_isr_handler(void){uint8_t sc=port_byte_in(0x60);if(sc==0x2A||sc==0x36){shift_down=1;goto done;}if(sc==0xAA||sc==0xB6){shift_down=0;goto done;}if(sc==0x3A){caps_lock=!caps_lock;goto done;}if(sc&0x80)goto done;if(sc==0x1C){execute();goto done;}if(sc==0x0E){if(input_len>0){input[--input_len]=0;render();}goto done;}char c=key_char(sc);if(c&&input_len<MAX_INPUT-1){input[input_len++]=c;input[input_len]=0;render();}done:port_byte_out(0x20,0x20);}
void keyboard_on_timer_tick(void){cursor_blink++;}
