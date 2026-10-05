#include "ui.h"
#include "mouse.h"
#include "keyboard.h"
#include "terminal.h"
typedef struct __attribute__((packed)){unsigned int type,size;unsigned long long addr;unsigned int pitch,width,height;unsigned char bpp,type2;unsigned short reserved;} fbtag_t;
static volatile unsigned int*fb;static unsigned int pitch,width,height;static int mx=400,my=300;
static void px(int x,int y,unsigned int c){if(!fb||x<0||y<0||(unsigned)x>=width||(unsigned)y>=height)return;*(volatile unsigned int*)((unsigned char*)fb+y*pitch+x*4)=c;}
static void box(int x,int y,int w,int h,unsigned int c){for(int yy=0;yy<h;yy++)for(int xx=0;xx<w;xx++)px(x+xx,y+yy,c);}
static void label(int x,int y,const char*s){while(*s){if(*s!=' ')box(x,y,5,9,0x00ffffff);x+=7;s++;}}
static void cur(void){for(int i=0;i<12;i++)for(int j=0;j<=i;j++)px(mx+j,my+i,0x00ffffff);}
void ui_bind(unsigned long info){unsigned char*p=(unsigned char*)info+8;while(*(unsigned int*)p){unsigned int t=*(unsigned int*)p,s=*(unsigned int*)(p+4);if(t==8){fbtag_t*z=(fbtag_t*)p;if(z->type2==1&&z->bpp==32){fb=(volatile unsigned int*)(unsigned long)z->addr;pitch=z->pitch;width=z->width;height=z->height;}return;}p+=(s+7)&~7;}}
int ui_available(void){return fb!=0;}
void ui_run(void){box(0,0,width,height,0x00151a24);box(0,0,width,42,0x00232a38);box(0,height-34,width,34,0x00232a38);box(24,72,360,210,0x00202a36);box(24,72,360,30,0x002d3746);box(52,126,300,118,0x001b222c);box(52,126,300,2,0x004f9cff);label(40,82,"CUSTOMOS");label(68,138,"TERMINAL");label(68,160,"MOUSE READY");label(68,182,"MINIMAL DESKTOP");label(40,height-24,"CUSTOMOS");mouse_init();cur();for(;;){int dx,dy,b;while(mouse_poll(&dx,&dy,&b)){box(mx-1,my-1,14,14,0x00151a24);mx+=dx;my+=dy;if(mx<0)mx=0;if(my<0)my=0;if(mx>(int)width-2)mx=width-2;if(my>(int)height-2)my=height-2;if(b&1&&mx>=24&&mx<=384&&my>=72&&my<=282){terminal_init();terminal_clear();terminal_write("CustomOS terminal\n");return;}cur();}if(keyboard_getchar()>=0){terminal_init();terminal_clear();terminal_write("CustomOS terminal\n");return;}}}
