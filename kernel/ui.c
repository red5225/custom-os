#include "ui.h"
#include "mouse.h"
#include "keyboard.h"
#include "terminal.h"

typedef struct __attribute__((packed)){
    unsigned int type,size;
    unsigned long long addr;
    unsigned int pitch,width,height;
    unsigned char bpp,type2;
    unsigned short reserved;
} fbtag_t;

static volatile unsigned int *fb;
static unsigned int pitch,width,height;
static int mx=400,my=300;

static void pixel(int x,int y,unsigned int c){
    if(!fb||x<0||y<0||(unsigned int)x>=width||(unsigned int)y>=height)return;
    *(volatile unsigned int*)((unsigned char*)fb+y*pitch+x*4)=c;
}
static void rect(int x,int y,int w,int h,unsigned int c){
    for(int yy=0;yy<h;yy++)for(int xx=0;xx<w;xx++)pixel(x+xx,y+yy,c);
}
static void glyph(int x,int y,char ch){
    if(ch==' ')return;
    unsigned int seed=(unsigned char)ch;
    for(int yy=0;yy<8;yy++)for(int xx=0;xx<5;xx++)
        if(((seed>>(xx+(yy&1)))^(yy*3))&1)pixel(x+xx,y+yy,0x00ffffff);
}
static void text(int x,int y,const char *s){
    while(*s){glyph(x,y,*s++);x+=7;}
}
static void cursor(void){
    for(int i=0;i<12;i++)for(int j=0;j<=i;j++)pixel(mx+j,my+i,0x00ffffff);
}
void ui_bind(unsigned long info){
    unsigned char *p=(unsigned char*)info+8;
    while(*(unsigned int*)p){
        unsigned int type=*(unsigned int*)p;
        unsigned int size=*(unsigned int*)(p+4);
        if(type==8){
            fbtag_t *t=(fbtag_t*)p;
            if(t->bpp==32&&t->type2==1){
                fb=(volatile unsigned int*)(unsigned long)t->addr;
                pitch=t->pitch;width=t->width;height=t->height;
            }
        }
        p+=(size+7)&~7;
    }
}
int ui_available(void){return fb!=0;}
void ui_run(void){
    rect(0,0,width,height,0x00141a22);
    rect(0,0,width,44,0x00242d38);
    rect(0,height-36,width,36,0x00242d38);
    rect(22,68,370,220,0x001e2730);
    rect(22,68,370,32,0x002c3743);
    rect(48,116,318,138,0x00171e25);
    rect(48,116,318,2,0x0055aaff);
    text(38,80,"CUSTOMOS");
    text(64,132,"TERMINAL");
    text(64,154,"MOUSE + KEYBOARD");
    text(64,176,"MINIMAL DESKTOP");
    text(38,height-24,"CUSTOMOS");
    mouse_init();
    cursor();
    for(;;){
        int dx,dy,b;
        if(mouse_poll(&dx,&dy,&b)){
            rect(mx-1,my-1,14,14,0x00141a22);
            mx+=dx;my+=dy;
            if(mx<0)mx=0;if(my<0)my=0;
            if(mx>(int)width-13)mx=width-13;
            if(my>(int)height-13)my=height-13;
            if((b&1)&&mx>=22&&mx<=392&&my>=68&&my<=288){
                terminal_init();terminal_clear();
                terminal_write("CustomOS terminal\n");
                terminal_write("UI closed. Keyboard is active.\n");
                return;
            }
            cursor();
        }
        int c=keyboard_getchar();
        if(c>=0){
            terminal_init();terminal_clear();
            terminal_write("CustomOS terminal\n");
            terminal_write("Keyboard detected. UI closed.\n");
            return;
        }
    }
}
