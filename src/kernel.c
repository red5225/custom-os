#include <stdint.h>

static volatile uint8_t *const VGA = (uint8_t*)0xA0000;

static void pixel(int x, int y, uint8_t c) {
    if ((unsigned)x < 320 && (unsigned)y < 200) VGA[y * 320 + x] = c;
}
static void rect(int x,int y,int w,int h,uint8_t c){for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)pixel(xx,yy,c);}
static void frame(int x,int y,int w,int h,uint8_t c){for(int i=0;i<w;i++){pixel(x+i,y,c);pixel(x+i,y+h-1,c);}for(int i=0;i<h;i++){pixel(x,y+i,c);pixel(x+w-1,y+i,c);}}

static void letter(int x,int y,char ch,uint8_t c) {
    static const uint8_t f[][7]={{0,0,0,0,0,0,0},{31,17,17,31,17,17,17},{30,17,17,30,17,17,30},{14,17,16,16,16,17,14},{30,17,17,17,17,17,30},{31,16,16,30,16,16,31},{31,16,16,30,16,16,16},{14,17,16,23,17,17,14},{17,17,17,31,17,17,17},{31,4,4,4,4,4,31},{7,2,2,2,18,18,12},{17,18,20,24,20,18,17},{16,16,16,16,16,16,31},{17,27,21,17,17,17,17},{17,25,21,19,17,17,17},{14,17,17,17,17,17,14},{30,17,17,30,16,16,16},{14,17,17,17,21,18,13},{30,17,17,30,20,18,17},{15,16,16,14,1,1,30},{17,17,17,17,17,17,14},{17,17,17,17,17,10,4},{17,17,17,21,21,27,17},{17,17,10,4,10,17,17},{17,17,10,4,4,4,4},{31,1,2,4,8,16,31}};
    int i=(ch>='A'&&ch<='Z')?ch-'A'+1:0;
    for(int r=0;r<7;r++)for(int b=0;b<5;b++)if(f[i][r]&(1<<(4-b)))pixel(x+b,y+r,c);
}
static void text(int x,int y,const char*s,uint8_t c){while(*s){letter(x,y,*s++,c);x+=6;}}

void kmain(void){
    rect(0,0,320,200,1); rect(0,0,320,20,8);
    rect(8,5,10,10,14); text(24,6,"CUSTOM OS",15); text(268,6,"UTM",15);
    rect(10,32,300,42,7); frame(10,32,300,42,15);
    rect(18,40,26,26,14); text(52,43,"WELCOME",15); text(52,53,"TO YOUR DESKTOP",15);
    rect(10,84,94,70,8); frame(10,84,94,70,15); text(20,96,"FILES",15); text(20,110,"READY",14);
    rect(113,84,94,70,8); frame(113,84,94,70,15); text(123,96,"SYSTEM",15); text(123,110,"ONLINE",14);
    rect(216,84,94,70,8); frame(216,84,94,70,15); text(226,96,"TOOLS",15); text(226,110,"READY",14);
    rect(0,181,320,19,8); text(8,187,"CUSTOM OS",15); text(254,187,"BOOTED",14);
    for(;;)__asm__ volatile("hlt");
}
