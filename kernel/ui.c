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

static void px(int x,int y,unsigned int c){
	if(!fb||x<0||y<0||(unsigned)x>=width||(unsigned)y>=height)return;
	*(volatile unsigned int *)(fb+y*pitch+x*4)=c;
}
static void box(int x,int y,int w,int h,unsigned int c){
	for(int yy=0;yy<h;yy++)for(int xx=0;xx<w;xx++)px(x+xx,y+yy,c);
}
static void cursor(void){
	for(int i=0;i<12;i++)for(int j=0;j<=i;j++)px(mx+j,my+i,0x00ffffff);
}
void ui_bind(unsigned long info){
	unsigned char *p=(unsigned char *)info+8;
	while(1){
		unsigned int type=*(unsigned int *)p;
		unsigned int size=*(unsigned int *)(p+4);
		if(type==0)break;
		if(type==8){
			fbtag_t *t=(fbtag_t *)p;
			if(t->bpp==32&&t->type2==1){
				fb=(volatile unsigned char *)(unsigned long)t->addr;
				pitch=t->pitch;width=t->width;height=t->height;
			}
			break;
		}
		p+=(size+7)&~7;
	}
}
int ui_available(void){return fb!=0&&width>=640&&height>=480;}
void ui_run(void){
	box(0,0,width,height,0x00151a24);
	box(0,0,width,42,0x00232a38);
	box(24,70,360,190,0x00202a36);
	box(24,70,360,30,0x002d3746);
	box(48,118,312,2,0x004f9cff);
	box(24,height-34,width,34,0x00232a38);
	mouse_init();cursor();
	for(;;){
		int dx,dy,b;
		if(mouse_poll(&dx,&dy,&b)){
			box(mx-2,my-2,16,16,0x00151a24);
			mx+=dx;my+=dy;
			if(mx<0)mx=0;if(my<0)my=0;
			if(mx>(int)width-12)mx=width-12;
			if(my>(int)height-12)my=height-12;
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
