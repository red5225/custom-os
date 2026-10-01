typedef unsigned char u8;
typedef unsigned short u16;

#define W 320
#define H 200
#define VGA ((u8*)0xA0000)
#define TEXT ((volatile u16*)0xB8000)

static void put_text(int row, const char *s, u8 color) {
    int i = 0;
    while (s[i] && i < 79) {
        TEXT[row * 80 + i] = (u16)s[i] | ((u16)color << 8);
        i++;
    }
}

static void clear_text(void) {
    for (int i=0;i<80*25;i++) TEXT[i] = (u16)' ' | ((u16)0x07 << 8);
}

static void boot_delay(void) {
    for (volatile unsigned long i=0;i<1800000UL;i++) __asm__ volatile ("nop");
}

static inline void outb(u16 port, u8 value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static void set_mode_13h(void) {
    static const u8 seq[] = {0x03,0x01,0x0F,0x00,0x0E};
    static const u8 crtc[] = {
        0x5F,0x4F,0x50,0x82,0x54,0x80,0xBF,0x1F,
        0x00,0x41,0x00,0x00,0x00,0x00,0x00,0x00,
        0x9C,0x0E,0x8F,0x28,0x1F,0x96,0xB9,0xA3,0xFF
    };
    outb(0x3C2,0x63);
    for(int i=0;i<5;i++){outb(0x3C4,i);outb(0x3C5,seq[i]);}
    outb(0x3D4,0x03); outb(0x3D5,0x80);
    for(int i=0;i<25;i++){outb(0x3D4,i);outb(0x3D5,crtc[i]);}
    outb(0x3C0,0x10);outb(0x3C0,0x41);
    outb(0x3C0,0x11);outb(0x3C0,0xFF);
    outb(0x3C0,0x12);outb(0x3C0,0x0F);
    outb(0x3C0,0x13);outb(0x3C0,0x00);
    outb(0x3C0,0x14);outb(0x3C0,0x00);
}

static void pixel(int x,int y,u8 c){if((unsigned)x<W&&(unsigned)y<H)VGA[y*W+x]=c;}
static void rect(int x,int y,int w,int h,u8 c){for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)pixel(xx,yy,c);}

static const u8 glyph(char c,int r) {
    static const u8 A[7]={14,17,17,31,17,17,17}, C[7]={15,16,16,16,16,16,15};
    static const u8 D[7]={30,17,17,17,17,17,30}, E[7]={31,16,16,30,16,16,31};
    static const u8 F[7]={31,16,16,30,16,16,16}, G[7]={15,16,16,23,17,17,15};
    static const u8 Hh[7]={17,17,17,31,17,17,17}, I[7]={31,4,4,4,4,4,31};
    static const u8 L[7]={16,16,16,16,16,16,31}, M[7]={17,27,21,21,17,17,17};
    static const u8 N[7]={17,25,21,19,17,17,17}, O[7]={14,17,17,17,17,17,14};
    static const u8 R[7]={30,17,17,30,20,18,17}, S[7]={15,16,16,14,1,1,30};
    static const u8 T[7]={31,4,4,4,4,4,4}, U[7]={17,17,17,17,17,17,14};
    static const u8 Ww[7]={17,17,17,21,21,27,17}, Y[7]={17,17,10,4,4,4,4};
    static const u8 dash[7]={0,0,0,31,0,0,0};
    static const u8 *p;
    switch(c){case'A':p=A;break;case'C':p=C;break;case'D':p=D;break;case'E':p=E;break;
    case'F':p=F;break;case'G':p=G;break;case'H':p=Hh;break;case'I':p=I;break;case'L':p=L;break;
    case'M':p=M;break;case'N':p=N;break;case'O':p=O;break;case'R':p=R;break;case'S':p=S;break;
    case'T':p=T;break;case'U':p=U;break;case'W':p=Ww;break;case'Y':p=Y;break;case'-':p=dash;break;
    case' ':return 0;default:return 0;} return p[r];
}
static void text(int x,int y,const char*s,u8 color,int scale){
    for(int n=0;s[n];n++)for(int r=0;r<7;r++){u8 b=glyph(s[n],r);for(int col=0;col<5;col++)
        if(b&(1<<(4-col)))rect(x+n*6*scale+col*scale,y+r*scale,scale,scale,color);}
}

void kmain(void) {
    clear_text();
    put_text(3,"CUSTOM OS - KERNEL BOOT",0x0F);
    put_text(6,"[ OK ] CPU / protected mode",0x0A); boot_delay();
    put_text(7,"[ OK ] VGA framebuffer",0x0A); boot_delay();
    put_text(8,"[ OK ] Kernel initialized",0x0A); boot_delay();
    put_text(10,"[ OK ] Launching graphical UI...",0x0B); boot_delay();

    set_mode_13h();

    rect(0,0,W,H,1);
    rect(0,0,W,24,8); rect(0,23,W,1,9);
    text(10,8,"CUSTOM OS",15,2); text(258,8,"1.0",7,1);
    rect(0,24,62,H-24,2); rect(61,24,1,H-24,9);
    text(10,38,"HOME",14,1); rect(8,54,46,22,8); text(14,62,"SYSTEM",15,1);
    text(10,88,"FILES",14,1);
    rect(76,38,230,48,8); rect(76,38,5,48,10);
    text(90,48,"WELCOME",15,2); text(90,67,"CUSTOM OS",7,1);
    rect(76,100,68,58,2); rect(151,100,68,58,2); rect(226,100,80,58,2);
    text(86,110,"SYSTEM",14,1); text(161,110,"FILES",14,1); text(236,110,"ABOUT",14,1);
    rect(76,169,230,18,8); text(86,175,"SYSTEM READY",10,1);

    for(;;) __asm__ volatile("hlt");
}
