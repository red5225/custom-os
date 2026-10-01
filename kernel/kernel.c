typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define TEXT ((volatile u16*)0xB8000)
#define VGA ((volatile u8*)0xA0000)
#define SCREEN_W 320
#define SCREEN_H 200

static inline void outb(u16 p,u8 v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));}
static inline u8 inb(u16 p){u8 v;__asm__ volatile("inb %1,%0":"=a"(v):"Nd"(p));return v;}

static void text_clear(void){
    for(int i=0;i<80*25;i++) TEXT[i]=(u16)' '|((u16)0x07<<8);
}
static void text_put(int row,int col,const char*s,u8 color){
    while(*s && col<80) TEXT[row*80+col++]=(u16)*s++|((u16)color<<8);
}
static int eq(const char*a,const char*b){
    int i=0; while(a[i]&&b[i]&&a[i]==b[i]) i++;
    return a[i]==0&&b[i]==0;
}
static char key(u8 s){
    if(s>=0x02&&s<=0x0D) return "1234567890-="[s-0x02];
    if(s>=0x10&&s<=0x19) return "qwertyuiop"[s-0x10];
    if(s>=0x1E&&s<=0x26) return "asdfghjkl"[s-0x1E];
    if(s>=0x2C&&s<=0x35) return "zxcvbnm,./"[s-0x2C];
    if(s==0x39) return ' ';
    return 0;
}

static void pit_init(void){
    outb(0x43,0x34);
    outb(0x40,1193&255);
    outb(0x40,(1193>>8)&255);
}
static u16 pit(void){
    outb(0x43,0);
    u16 lo=inb(0x40),hi=inb(0x40);
    return (u16)(lo|(hi<<8));
}
static void wait_seconds(int seconds){
    u16 prev=pit();
    u32 ticks=0;
    while(ticks<(u32)seconds*1000U){
        u16 now=pit();
        if(now>prev) ticks++;
        prev=now;
    }
}

static void mode13(void){
    static const u8 seq[5]={3,1,15,0,14};
    static const u8 crt[25]={95,79,80,130,84,128,191,31,0,65,0,0,0,0,0,0,156,14,143,40,31,150,185,163,255};
    outb(0x3C2,0x63);
    for(int i=0;i<5;i++){outb(0x3C4,i);outb(0x3C5,seq[i]);}
    outb(0x3D4,3);outb(0x3D5,0x80);
    for(int i=0;i<25;i++){outb(0x3D4,i);outb(0x3D5,crt[i]);}
    outb(0x3C0,0x10);outb(0x3C0,0x41);
    outb(0x3C0,0x11);outb(0x3C0,0xFF);
    outb(0x3C0,0x12);outb(0x3C0,0x0F);
    outb(0x3C0,0x13);outb(0x3C0,0);
    outb(0x3C0,0x14);outb(0x3C0,0);
}
static void mode03(void){
    static const u8 seq[5]={3,1,15,0,6};
    static const u8 crt[25]={95,79,80,130,85,159,191,31,0,79,0,0,0,0,0,0,156,14,143,40,31,150,185,163,255};
    outb(0x3C2,0x67);
    for(int i=0;i<5;i++){outb(0x3C4,i);outb(0x3C5,seq[i]);}
    outb(0x3D4,3);outb(0x3D5,0x80);
    for(int i=0;i<25;i++){outb(0x3D4,i);outb(0x3D5,crt[i]);}
    outb(0x3C0,0x20);
    text_clear();
}
static void fill(int x,int y,int w,int h,u8 c){
    for(int yy=y;yy<y+h;yy++) for(int xx=x;xx<x+w;xx++)
        if((unsigned)xx<SCREEN_W&&(unsigned)yy<SCREEN_H) VGA[yy*SCREEN_W+xx]=c;
}
static void mouse_init(void){
    u32 t=0;
    while((inb(0x64)&2)&&++t<1000000){}
    outb(0x64,0xA8);
    t=0; while((inb(0x64)&2)&&++t<1000000){}
    outb(0x64,0xD4);
    t=0; while((inb(0x64)&2)&&++t<1000000){}
    outb(0x60,0xF4);
    t=0; while(!(inb(0x64)&1)&&++t<1000000){}
    if(t<1000000) inb(0x60);
}
static int mouse_packet(u8 p[3]){
    if(!(inb(0x64)&1)) return 0;
    p[0]=inb(0x60);
    if(!(p[0]&8)) return 0;
    u32 t=0;
    while(!(inb(0x64)&1)&&++t<1000000) {}
    if(t>=1000000) return 0;
    p[1]=inb(0x60);
    t=0;
    while(!(inb(0x64)&1)&&++t<1000000) {}
    if(t>=1000000) return 0;
    p[2]=inb(0x60);
    return 1;
}

static int ui(void){
    mode13();
    mouse_init();

    /* Safe UI: no driver or filesystem dependency. */
    fill(0,0,320,200,1);
    fill(0,0,320,24,8);
    fill(0,24,62,176,2);
    fill(75,42,220,115,8);
    fill(108,103,154,42,10);
    fill(112,107,146,34,12);

    /* 10-second watchdog. A click on the large center button returns normally. */
    int mx=160,my=95;
    u8 p[3];
    u16 prev=pit();
    u32 ticks=0;

    while(ticks<10000){
        u16 now=pit();
        if(now>prev) ticks++;
        prev=now;

        if(mouse_packet(p)){
            int dx=(p[0]&0x10)?(int)(signed char)p[1]:(int)p[1];
            int dy=(p[0]&0x20)?(int)(signed char)p[2]:(int)p[2];
            mx+=dx; my-=dy;
            if(mx<0)mx=0; if(mx>316)mx=316;
            if(my<0)my=0; if(my>196)my=196;

            if((p[0]&1)&&mx>=108&&mx<262&&my>=103&&my<145){
                mode03();
                text_put(1,2,"CUSTOM OS UI",0x0B);
                text_put(3,2,"UI BUTTON CLICKED - OK",0x0A);
                text_put(5,2,"TEXT MODE IS READY",0x07);
                text_put(7,2,"> ",0x0F);
                return 1;
            }
        }
        fill(mx,my,4,4,15);
    }

    mode03();
    text_put(1,2,"CUSTOM OS - TEXT MODE",0x0B);
    text_put(3,2,"UI ERROR: NO BUTTON CLICKED WITHIN 10 SECONDS.",0x0C);
    text_put(5,2,"RETURNED TO TEXT MODE SO THE SYSTEM STAYS USABLE.",0x0E);
    text_put(7,2,"TYPE 'ui' TO TRY THE UI AGAIN.",0x07);
    text_put(9,2,"> ",0x0F);
    return 0;
}

static void shell(void){
    text_clear();
    text_put(1,2,"CUSTOM OS",0x0B);
    text_put(3,2,"HI FROM CUSTOM OS",0x0A);
    text_put(5,2,"TEXT MODE KERNEL ONLINE",0x0F);
    text_put(7,2,"Type 'help' for commands.",0x07);

    char cmd[64];
    int n=0,row=9;
    text_put(row,2,"> ",0x0F);

    for(;;){
        if(!(inb(0x64)&1)) continue;
        u8 s=inb(0x60);
        if(s&0x80) continue;

        if(s==0x1C){
            cmd[n]=0;
            if(eq(cmd,"help")){
                text_put(++row,2,"help - show commands",0x07);
                text_put(++row,2,"ui   - launch optional graphical UI",0x07);
                text_put(++row,2,"clear- clear terminal",0x07);
            }else if(eq(cmd,"ui")){
                ui();
                row=9; n=0;
                text_put(row,2,"> ",0x0F);
                continue;
            }else if(eq(cmd,"clear")){
                text_clear(); row=1;
            }else if(n){
                text_put(++row,2,"Unknown command. Type help.",0x0C);
            }
            if(row>21){text_clear();row=1;}
            text_put(++row,2,"> ",0x0F);
            n=0;
        }else if(s==0x0E&&n>0){
            n--;
            TEXT[row*80+3+n]=(u16)' '|((u16)0x07<<8);
        }else{
            char c=key(s);
            if(c&&n<60){
                cmd[n++]=c;
                TEXT[row*80+3+n-1]=(u16)c|((u16)0x0F<<8);
            }
        }
    }
}

void kmain(void){
    pit_init();
    shell();
    for(;;)__asm__ volatile("hlt");
}
