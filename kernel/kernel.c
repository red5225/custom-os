typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define TEXT ((volatile u16*)0xB8000)
#define VGA ((volatile u8*)0xA0000)
#define W 320
#define H 200

static inline void outb(u16 p,u8 v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));}
static inline u8 inb(u16 p){u8 v;__asm__ volatile("inb %1,%0":"=a"(v):"Nd"(p));return v;}

static void clear_text(void){for(int i=0;i<2000;i++)TEXT[i]=(u16)' '|((u16)7<<8);}
static void put(int r,int c,const char*s,u8 col){while(*s&&c<80)TEXT[r*80+c++]=(u16)*s++|((u16)col<<8);}
static int eq(const char*a,const char*b){int i=0;while(a[i]&&b[i]&&a[i]==b[i])i++;return a[i]==0&&b[i]==0;}

static char key(u8 s){
    if(s>=0x02&&s<=0x0D)return "1234567890-="[s-2];
    if(s>=0x10&&s<=0x19)return "qwertyuiop"[s-0x10];
    if(s>=0x1E&&s<=0x26)return "asdfghjkl"[s-0x1E];
    if(s>=0x2C&&s<=0x35)return "zxcvbnm,./"[s-0x2C];
    if(s==0x39)return ' ';
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

static void mode13(void){
    static const u8 s[5]={3,1,15,0,14};
    static const u8 c[25]={95,79,80,130,84,128,191,31,0,65,0,0,0,0,0,0,156,14,143,40,31,150,185,163,255};
    outb(0x3C2,0x63);
    for(int i=0;i<5;i++){outb(0x3C4,i);outb(0x3C5,s[i]);}
    outb(0x3D4,3);outb(0x3D5,0x80);
    for(int i=0;i<25;i++){outb(0x3D4,i);outb(0x3D5,c[i]);}
    outb(0x3C0,0x10);outb(0x3C0,0x41);
    outb(0x3C0,0x11);outb(0x3C0,0xFF);
    outb(0x3C0,0x12);outb(0x3C0,0x0F);
    outb(0x3C0,0x13);outb(0x3C0,0);
    outb(0x3C0,0x14);outb(0x3C0,0);
}
static void mode03(void){
    static const u8 s[5]={3,1,15,0,6};
    static const u8 c[25]={95,79,80,130,85,159,191,31,0,79,0,0,0,0,0,0,156,14,143,40,31,150,185,163,255};
    outb(0x3C2,0x67);
    for(int i=0;i<5;i++){outb(0x3C4,i);outb(0x3C5,s[i]);}
    outb(0x3D4,3);outb(0x3D5,0x80);
    for(int i=0;i<25;i++){outb(0x3D4,i);outb(0x3D5,c[i]);}
    outb(0x3C0,0x20);
    clear_text();
}

static void px(int x,int y,u8 c){if((unsigned)x<W&&(unsigned)y<H)VGA[y*W+x]=c;}
static void box(int x,int y,int w,int h,u8 c){for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)px(xx,yy,c);}

static const u8 glyph(char c,int r){
    static const u8 A[7]={14,17,17,31,17,17,17},C[7]={15,16,16,16,16,16,15};
    static const u8 D[7]={30,17,17,17,17,17,30},E[7]={31,16,16,30,16,16,31};
    static const u8 F[7]={31,16,16,30,16,16,16},G[7]={15,16,16,23,17,17,15};
    static const u8 H[7]={17,17,17,31,17,17,17},I[7]={31,4,4,4,4,4,31};
    static const u8 L[7]={16,16,16,16,16,16,31},M[7]={17,27,21,21,17,17,17};
    static const u8 N[7]={17,25,21,19,17,17,17},O[7]={14,17,17,17,17,17,14};
    static const u8 R[7]={30,17,17,30,20,18,17},S[7]={15,16,16,14,1,1,30};
    static const u8 T[7]={31,4,4,4,4,4,4},U[7]={17,17,17,17,17,17,14};
    static const u8 Ww[7]={17,17,21,21,21,27,17},Y[7]={17,17,10,4,4,4,4};
    const u8*p=0;
    switch(c){
        case'A':p=A;break;case'C':p=C;break;case'D':p=D;break;case'E':p=E;break;
        case'F':p=F;break;case'G':p=G;break;case'H':p=H;break;case'I':p=I;break;
        case'L':p=L;break;case'M':p=M;break;case'N':p=N;break;case'O':p=O;break;
        case'R':p=R;break;case'S':p=S;break;case'T':p=T;break;case'U':p=U;break;
        case'W':p=Ww;break;case'Y':p=Y;break;default:return 0;
    }
    return p[r];
}
static void txt(int x,int y,const char*s,u8 c){
    for(int n=0;s[n];n++)for(int r=0;r<7;r++){u8 b=glyph(s[n],r);for(int k=0;k<5;k++)if(b&(1<<(4-k)))px(x+n*6+k,y+r,c);}
}

static void mouse_init(void){
    while(inb(0x64)&2){}
    outb(0x64,0xA8);
    while(inb(0x64)&2){}
    outb(0x64,0xD4);
    while(inb(0x64)&2){}
    outb(0x60,0xF6);
    while(!(inb(0x64)&1)){}
    inb(0x60);
    while(inb(0x64)&2){}
    outb(0x64,0xD4);
    while(inb(0x64)&2){}
    outb(0x60,0xF4);
    while(!(inb(0x64)&1)){}
    inb(0x60);
}
static int mouse_packet(u8*p){
    if(!(inb(0x64)&1))return 0;
    p[0]=inb(0x60);
    if(!(p[0]&8))return 0;
    if(!(inb(0x64)&1))return 0;p[1]=inb(0x60);
    if(!(inb(0x64)&1))return 0;p[2]=inb(0x60);
    return 1;
}

static int launch_ui(void){
    mode13();mouse_init();
    box(0,0,W,H,1);box(0,0,W,24,8);box(0,24,60,H-24,2);
    txt(10,8,"CUSTOM OS",15);txt(10,38,"HOME",14);txt(10,58,"SYSTEM",14);txt(10,78,"FILES",14);
    box(85,45,190,55,8);txt(105,57,"WELCOME",15);
    box(110,105,100,32,10);txt(126,116,"ENTER",0);
    txt(90,150,"CLICK THE BUTTON",15);
    txt(100,162,"WITH MOUSE",15);

    int mx=160,my=95;u8 p[3];u16 prev=pit();u32 ticks=0;
    while(ticks<10000){
        u16 now=pit();if(now>prev)ticks++;prev=now;
        if(mouse_packet(p)){
            int dx=(p[0]&0x10)?(int)(signed char)p[1]:(int)p[1];
            int dy=(p[0]&0x20)?(int)(signed char)p[2]:(int)p[2];
            mx+=dx;my-=dy;
            if(mx<0)mx=0;if(mx>316)mx=316;if(my<0)my=0;if(my>196)my=196;
            /* redraw UI to erase the old cursor */
            box(0,0,W,H,1);box(0,0,W,24,8);box(0,24,60,H-24,2);
            txt(10,8,"CUSTOM OS",15);txt(10,38,"HOME",14);txt(10,58,"SYSTEM",14);txt(10,78,"FILES",14);
            box(85,45,190,55,8);txt(105,57,"WELCOME",15);box(110,105,100,32,10);txt(126,116,"ENTER",0);
            txt(90,150,"CLICK THE BUTTON",15);txt(100,162,"WITH MOUSE",15);
            box(mx,my,4,4,15);
            if((p[0]&1)&&mx>=110&&mx<210&&my>=105&&my<137){
                mode03();
                put(1,2,"CUSTOM OS - TEXT MODE",0x0B);
                put(3,2,"UI BUTTON CLICKED - OK",0x0A);
                put(5,2,"> ",0x0F);
                return 1;
            }
        }
    }
    mode03();
    put(1,2,"CUSTOM OS - TEXT MODE",0x0B);
    put(3,2,"UI ERROR: NO BUTTON CLICKED WITHIN 10 SECONDS.",0x0C);
    put(5,2,"RETURNED TO TEXT MODE SAFELY.",0x0E);
    put(7,2,"TYPE 'ui' TO TRY AGAIN.",0x07);
    put(9,2,"> ",0x0F);
    return 0;
}

static void shell(void){
    clear_text();
    put(1,2,"CUSTOM OS",0x0B);
    put(3,2,"TEXT MODE KERNEL",0x0F);
    put(5,2,"HI FROM CUSTOM OS",0x0A);
    put(7,2,"Type 'help' for commands.",0x07);
    int row=9,n=0;char cmd[64];
    put(row,2,"> ",0x0F);
    for(;;){
        if(!(inb(0x64)&1))continue;
        u8 s=inb(0x60);
        if(s&0x80)continue;
        if(s==0x1C){
            cmd[n]=0;
            if(eq(cmd,"help")){
                put(++row,2,"help - commands",0x07);
                put(++row,2,"ui   - launch graphical UI",0x07);
                put(++row,2,"clear- clear terminal",0x07);
            }else if(eq(cmd,"ui")){
                launch_ui();row=9;n=0;put(row,2,"> ",0x0F);continue;
            }else if(eq(cmd,"clear")){
                clear_text();row=1;
            }else if(n)put(++row,2,"Unknown command. Type help.",0x0C);
            if(row>21){clear_text();row=1;}
            put(++row,2,"> ",0x0F);n=0;
        }else if(s==0x0E&&n){
            n--;TEXT[row*80+3+n]=(u16)' '|((u16)7<<8);
        }else{
            char c=key(s);
            if(c&&n<60){cmd[n++]=c;TEXT[row*80+3+n-1]=(u16)c|((u16)15<<8);}
        }
    }
}

void kmain(void){
    pit_init();
    shell();
}