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

static volatile unsigned char *fb;
static unsigned int pitch,width,height;
static int mx=400,my=300;

static void pixel(int x,int y,unsigned int c){
    if(!fb||x<0||y<0||(unsigned int)x>=width||(unsigned int)y>=height)return;
    *(volatile unsigned int*)(fb+y*pitch+x*4)=c;
}
static void rect(int x,int y,int w,int h,unsigned int c){
    for(int yy=0;yy<h;yy++)for(int xx=0;xx<w;xx++)pixel(x+xx,y+yy,c);
}
static void label(int x,int y,const char *s){
    while(*s){
        if(*s!=' ')rect(x,y,6,10,0x00ffffff);
        x+=8;s++;
    }
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
            if(t->type2!=1||t->bpp!=32)return;
            fb=(volatile unsigned char*)(unsigned long)t->addr;
            pitch=t->pitch;width=t->width;height=t->height;
            return;
        }
        p+=((size+7)&~7);
    }
}
int ui_available(void){return fb!=0;}

void ui_run(void){
    rect(0,0,width,height,0x00151a24);
    rect(0,0,width,44,0x00232a38);
    rect(0,height-36,width,36,0x00232a38);
    rect(24,72,390,220,0x00202a36);
    rect(24,72,390,32,0x002d3746);
    rect(52,128,334,132,0x001b222c);
    rect(52,128,334,2,0x004f9cff);
    label(40,83,"CUSTOMOS");
    label(68,140,"MINIMAL DESKTOP");
    label(68,162,"MOUSE READY");
    label(68,184,"CLICK PANEL TO OPEN TERMINAL");
    label(40,height-25,"CUSTOMOS");
    cursor();
    mouse_init();

    for(;;){
        int dx,dy,b;
        while(mouse_poll(&dx,&dy,&b)){
            mx+=dx; my+=dy;
            if(mx<0)mx=0;if(my<0)my=0;
            if(mx>(int)width-2)mx=width-2;
            if(my>(int)height-2)my=height-2;
            rect(0,0,width,height,0x00151a24);
            rect(0,0,width,44,0x00232a38);
            rect(0,height-36,width,36,0x00232a38);
            rect(24,72,390,220,0x00202a36);
            rect(24,72,390,32,0x002d3746);
            rect(52,128,334,132,0x001b222c);
            rect(52,128,334,2,0x004f9cff);
            label(40,83,"CUSTOMOS");
            label(68,140,"MINIMAL DESKTOP");
            label(68,162,"MOUSE READY");
            label(68,184,"CLICK PANEL TO OPEN TERMINAL");
            label(40,height-25,"CUSTOMOS");
            if(b&1 && mx>=24&&mx<=414&&my>=72&&my<=292){
                terminal_init();
                terminal_clear();
                terminal_write("CustomOS terminal
");
                terminal_write("UI closed. Keyboard is active.
");
                return;
            }
            cursor();
        }
        if(keyboard_getchar()>=0){
            terminal_init();
            terminal_clear();
            terminal_write("CustomOS terminal
");
            terminal_write("Keyboard detected. UI closed.
");
            return;
        }
    }
}
