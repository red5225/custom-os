#include "input.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#define TEXT ((volatile u16*)0xB8000)
#define VGA ((volatile u8*)0xA0000)
#define SCREEN_W 320
#define SCREEN_H 200

static inline void outb(u16 p,u8 v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));}
static inline u8 inb(u16 p){u8 v;__asm__ volatile("inb %1,%0":"=a"(v):"Nd"(p));return v;}
static void text_clear(void){for(int i=0;i<2000;i++)TEXT[i]=(u16)' '|((u16)7<<8);}
static void text_put(int r,int c,const char*s,u8 col){if(r<0||r>=25||c<0||!s)return;while(*s&&c<80)TEXT[r*80+c++]=(u16)*s++|((u16)col<<8);}
static void mode03(void){
    static const u8 seq[5]={3,1,15,0,6}; static const u8 crt[25]={95,79,80,130,85,159,191,31,0,79,0,0,0,0,0,0,156,14,143,40,31,150,185,163,255};
    outb(0x3C2,0x67); for(int i=0;i<5;i++){outb(0x3C4,i);outb(0x3C5,seq[i]);}
    outb(0x3D4,3);outb(0x3D5,0x80); for(int i=0;i<25;i++){outb(0x3D4,i);outb(0x3D5,crt[i]);}
    outb(0x3C0,0x20); text_clear();
}
static void mode13(void){
    static const u8 seq[5]={3,1,15,0,14}; static const u8 crt[25]={95,79,80,130,84,128,191,31,0,65,0,0,0,0,0,0,156,14,143,40,31,150,185,163,255};
    outb(0x3C2,0x63); for(int i=0;i<5;i++){outb(0x3C4,i);outb(0x3C5,seq[i]);}
    outb(0x3D4,3);outb(0x3D5,0x80); for(int i=0;i<25;i++){outb(0x3D4,i);outb(0x3D5,crt[i]);}
    outb(0x3C0,0x10);outb(0x3C0,0x41);outb(0x3C0,0x11);outb(0x3C0,0xFF);outb(0x3C0,0x12);outb(0x3C0,0x0F);outb(0x3C0,0x13);outb(0x3C0,0);outb(0x3C0,0x14);outb(0x3C0,0);
}
static void fill(int x,int y,int w,int h,u8 c){for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)if((unsigned)xx<320&&(unsigned)yy<200)VGA[yy*320+xx]=c;}

static int ui(void){
    mode13(); int mx=160,my=96,oldx=160,oldy=96; int dx,dy,l,r,m;
    fill(0,0,320,200,1);fill(0,0,320,24,8);fill(0,24,62,176,2);fill(75,42,220,115,8);fill(108,103,154,42,10);fill(112,107,146,34,12);fill(mx,my,5,5,15);
    /* The UI is deliberately finite: no input means a safe return to text mode. */
    for(u32 guard=0;guard<30000000U;guard++){
        if(mouse_read(&dx,&dy,&l,&r,&m)){
            mx+=dx;my+=dy;if(mx<0)mx=0;if(mx>315)mx=315;if(my<0)my=0;if(my>195)my=195;
            fill(oldx,oldy,5,5,8);fill(mx,my,5,5,15);oldx=mx;oldy=my;
            if(l&&mx>=108&&mx<262&&my>=103&&my<145){mode03();text_put(2,2,"UI OK - MOUSE CLICK RECEIVED",0x0A);text_put(4,2,"TEXT MODE READY",0x07);return 1;}
        }
        char c; if(keyboard_read(&c) && c=='\n'){mode03();text_put(2,2,"UI OK - KEYBOARD INPUT RECEIVED",0x0A);return 1;}
    }
    mode03();text_put(2,2,"UI TIMEOUT - NO INPUT RECEIVED",0x0C);text_put(4,2,"RETURNED TO TEXT MODE SAFELY",0x0E);return 0;
}

static void shell(void){
    text_clear();text_put(1,2,"CUSTOM OS",0x0B);text_put(3,2,"HI FROM CUSTOM OS",0x0A);text_put(5,2,"TEXT KERNEL ONLINE",0x0F);text_put(7,2,"help=commands  ui=graphics  clear=screen",0x07);
    char cmd[64];int n=0,row=9;text_put(row,2,"> ",0x0F);
    for(;;){
        char c; if(!keyboard_read(&c)) continue;
        if(c=='\n'){
            cmd[n]=0;
            if(n==4&&cmd[0]=='h'&&cmd[1]=='e'&&cmd[2]=='l'&&cmd[3]=='p'){text_put(++row,2,"help - commands",7);text_put(++row,2,"ui - mouse/graphics test",7);text_put(++row,2,"clear - clear screen",7);}
            else if(n==2&&cmd[0]=='u'&&cmd[1]=='i'){ui();row=9;n=0;text_put(row,2,"> ",15);continue;}
            else if(n==5&&cmd[0]=='c'&&cmd[1]=='l'&&cmd[2]=='e'&&cmd[3]=='a'&&cmd[4]=='r'){text_clear();row=1;}
            else if(n){text_put(++row,2,"Unknown command. Type help.",0x0C);}
            if(row>21){text_clear();row=1;}text_put(++row,2,"> ",15);n=0;
        }else if(c=='\b'&&n>0){n--;TEXT[row*80+3+n]=(u16)' '|((u16)7<<8);}
        else if(c>=32&&c<=126&&n<63){cmd[n++]=c;TEXT[row*80+3+n-1]=(u16)c|((u16)15<<8);}
    }
}
void kmain(void){input_init();shell();for(;;)__asm__ volatile("hlt");}
