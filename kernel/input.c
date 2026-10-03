#include "input.h"

static inline void outb(u16 p,u8 v){ __asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p)); }
static inline u8 inb(u16 p){ u8 v; __asm__ volatile("inb %1,%0":"=a"(v):"Nd"(p)); return v; }

static int wait_write(void){
    u32 t=0;
    while((inb(0x64)&2) && ++t<1000000) {}
    return t<1000000;
}
static int wait_read(void){
    u32 t=0;
    while(!(inb(0x64)&1) && ++t<1000000) {}
    return t<1000000;
}
static void flush(void){
    u32 t=0;
    while((inb(0x64)&1) && ++t<100000) (void)inb(0x60);
}
static void kcmd(u8 c){ if(wait_write()) outb(0x64,c); }
static void kdata(u8 d){ if(wait_write()) outb(0x60,d); }

static void keyboard_setup(void){
    kcmd(0xAD); /* disable keyboard */
    kcmd(0xA7); /* disable mouse */
    flush();

    /* Controller config: enable keyboard clock/data; no translation. */
    kcmd(0x20);
    u8 cfg=0;
    if(wait_read()) cfg=inb(0x60);
    cfg &= (u8)~0x40;
    cfg &= (u8)~0x01; /* polling driver: keep keyboard IRQ disabled */
    cfg &= (u8)~0x02; /* polling driver: keep mouse IRQ disabled */
    kcmd(0x60); kdata(cfg);

    kcmd(0xAE);

    /* Reset keyboard. Do not depend on the response; UTM firmware varies. */
    kdata(0xFF);
    if(wait_read()) (void)inb(0x60);
    flush();
}

static void mouse_setup(void){
    kcmd(0xA8);
    kcmd(0xD4);
    kdata(0xF4);
    if(wait_read()) (void)inb(0x60);
}

void input_init(void){
    __asm__ volatile("cli");
    keyboard_setup();
    mouse_setup();
    flush();
}

/* Scan-code set 1 and set 2 for the common US keys. */
static char set1(u8 s){
    static const char a[]="1234567890-=";
    static const char q[]="qwertyuiop";
    static const char z[]="asdfghjkl";
    static const char x[]="zxcvbnm,./";
    if(s>=0x02&&s<=0x0D) return a[s-0x02];
    if(s>=0x10&&s<=0x19) return q[s-0x10];
    if(s>=0x1E&&s<=0x26) return z[s-0x1E];
    if(s>=0x2C&&s<=0x35) return x[s-0x2C];
    if(s==0x39) return ' ';
    return 0;
}
static char set2(u8 s){
    switch(s){
        case 0x16:return '1'; case 0x1E:return '2'; case 0x26:return '3';
        case 0x25:return '4'; case 0x2E:return '5'; case 0x36:return '6';
        case 0x3D:return '7'; case 0x3E:return '8'; case 0x46:return '9';
        case 0x45:return '0'; case 0x4E:return '-'; case 0x55:return '=';
        case 0x15:return 'q'; case 0x1D:return 'w'; case 0x24:return 'e';
        case 0x2D:return 'r'; case 0x2C:return 't'; case 0x35:return 'y';
        case 0x3C:return 'u'; case 0x43:return 'i'; case 0x44:return 'o';
        case 0x4D:return 'p'; case 0x1C:return 'a'; case 0x1B:return 's';
        case 0x23:return 'd'; case 0x2B:return 'f'; case 0x34:return 'g';
        case 0x33:return 'h'; case 0x3B:return 'j'; case 0x42:return 'k';
        case 0x4B:return 'l'; case 0x3A:return 'm'; case 0x31:return 'n';
        case 0x21:return 'c'; case 0x32:return 'b'; case 0x1A:return 'z';
        case 0x22:return 'x'; case 0x2A:return 'v';
        case 0x29:return ' '; case 0x41:return ','; case 0x49:return '.';
        case 0x4A:return '/';
    }
    return 0;
}
int keyboard_read(char *out){
    u8 st=inb(0x64);
    if(!(st&1) || (st&0x20)) return 0;
    u8 s=inb(0x60);
    static int e0=0;
    static int release=0;
    if(s==0xE0){ e0=1; return 0; }
    if(s==0xF0){ release=1; return 0; }
    if(s&0x80){ release=1; s&=0x7F; }
    if(release){ release=0; e0=0; return 0; }
    if(e0){ e0=0; return 0; }
    char c=set1(s);
    if(!c) c=set2(s);
    if(c){ *out=c; return 1; }
    if(s==0x1C || s==0x5A) { *out='\n'; return 1; }
    if(s==0x0E || s==0x66) { *out='\b'; return 1; }
    return 0;
}

int mouse_read(int *dx,int *dy,int *left,int *right,int *middle){
    u8 st=inb(0x64);
    if(!(st&1) || !(st&0x20)) return 0;
    static u8 p[3]; static int n=0;
    u8 b=inb(0x60);
    if(n==0 && !(b&8)) return 0;
    p[n++]=b;
    if(n<3) return 0;
    n=0;
    *left=p[0]&1; *right=(p[0]>>1)&1; *middle=(p[0]>>2)&1;
    *dx=(p[0]&0x10)?(int)(signed char)p[1]:(int)p[1];
    *dy=(p[0]&0x20)?-(int)(signed char)p[2]:(int)-p[2];
    return 1;
}
