typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define VGA_TEXT ((volatile u16*)0xB8000)
#define COLS 80
#define ROWS 25

static inline void outb(u16 port,u8 value){
    __asm__ volatile("outb %0,%1"::"a"(value),"Nd"(port));
}
static inline u8 inb(u16 port){
    u8 value;
    __asm__ volatile("inb %1,%0":"=a"(value):"Nd"(port));
    return value;
}

static void clear(void){
    for(int i=0;i<COLS*ROWS;i++)
        VGA_TEXT[i]=(u16)' '|((u16)0x07<<8);
}
static void put(int row,int col,const char *s,u8 color){
    while(*s && row>=0 && row<ROWS && col<COLS){
        VGA_TEXT[row*COLS+col++]=(u16)*s++|((u16)color<<8);
    }
}
static int eq(const char *a,const char *b){
    int i=0;
    while(a[i] && b[i] && a[i]==b[i]) i++;
    return a[i]==0 && b[i]==0;
}

static char keychar(u8 sc){
    if(sc>=0x02 && sc<=0x0D) return "1234567890-="[sc-0x02];
    if(sc>=0x10 && sc<=0x19) return "qwertyuiop"[sc-0x10];
    if(sc>=0x1E && sc<=0x26) return "asdfghjkl"[sc-0x1E];
    if(sc>=0x2C && sc<=0x35) return "zxcvbnm,./"[sc-0x2C];
    if(sc==0x39) return ' ';
    return 0;
}

/* PIT channel 0 at about 1 kHz. */
static void pit_init(void){
    outb(0x43,0x34);
    outb(0x40,1193&255);
    outb(0x40,(1193>>8)&255);
}
static u16 pit_count(void){
    u16 lo,hi;
    outb(0x43,0);
    lo=inb(0x40);
    hi=inb(0x40);
    return (u16)(lo|((u16)hi<<8));
}
static void reset_timer(u16 *last,u32 *ticks){
    *last=pit_count();
    *ticks=0;
}
static int timer_10s(u16 *last,u32 *ticks){
    u16 now=pit_count();
    if(now>*last) (*ticks)++;
    *last=now;
    return *ticks>=10000;
}

static void text_screen(void){
    clear();
    put(1,2,"+--------------------------------------------------------------+",0x0B);
    put(2,2,"|                         CUSTOM OS                            |",0x0B);
    put(3,2,"|                    TEXT MODE KERNEL                         |",0x0B);
    put(4,2,"+--------------------------------------------------------------+",0x0B);
    put(6,2,"HI FROM CUSTOM OS",0x0A);
    put(8,2,"Commands:",0x0F);
    put(9,4,"help  - show commands",0x07);
    put(10,4,"ui    - open optional UI",0x07);
    put(11,4,"clear - clear the terminal",0x07);
    put(13,2,"The text shell is always available as the safe fallback.",0x0E);
}

static int ui(void){
    clear();
    put(1,2,"+------------------------------------------------------------------+",0x0B);
    put(2,2,"|                         CUSTOM OS UI                             |",0x0B);
    put(3,2,"+------------------------------------------------------------------+",0x0B);
    put(6,22,"+--------------------------+",0x0F);
    put(7,22,"|       [  LAUNCH  ]       |",0x0A);
    put(8,22,"+--------------------------+",0x0F);
    put(10,17,"Press ENTER or SPACE to activate the button.",0x07);
    put(11,20,"If nothing happens for 10 seconds,",0x07);
    put(12,20,"the OS returns to text mode.",0x07);

    u16 last=pit_count();
    u32 ticks=0;

    for(;;){
        if(timer_10s(&last,&ticks)){
            clear();
            put(2,2,"CUSTOM OS - TEXT MODE",0x0B);
            put(4,2,"UI ERROR: no button/input was received within 10 seconds.",0x0C);
            put(6,2,"Reason: UI activation timed out, so text mode was restored.",0x0E);
            put(8,2,"Type 'ui' to try the interface again.",0x07);
            return 0;
        }

        if(inb(0x64)&1){
            u8 sc=inb(0x60);
            if(!(sc&0x80)){
                if(sc==0x1C || sc==0x39){
                    clear();
                    put(2,2,"CUSTOM OS UI",0x0B);
                    put(5,25,"BUTTON ACTIVATED",0x0A);
                    put(7,17,"UI is running successfully.",0x07);
                    put(9,17,"Press ENTER to return to text mode.",0x07);
                    for(;;){
                        if(inb(0x64)&1){
                            u8 s=inb(0x60);
                            if(s==0x1C && !(s&0x80)) return 0;
                        }
                    }
                }
            }
        }
    }
}

static void shell(void){
    char cmd[64];
    int n=0;
    int row=15;

    text_screen();
    put(row,2,"> ",0x0F);

    for(;;){
        if(!(inb(0x64)&1)) continue;

        u8 sc=inb(0x60);
        if(sc&0x80) continue;

        if(sc==0x1C){
            cmd[n]=0;

            if(eq(cmd,"help")){
                put(++row,2,"help  - show commands",0x07);
                put(++row,2,"ui    - open optional UI",0x07);
                put(++row,2,"clear - clear terminal",0x07);
            }else if(eq(cmd,"ui")){
                ui();
                text_screen();
                row=15;
            }else if(eq(cmd,"clear")){
                text_screen();
                row=15;
            }else if(n){
                put(++row,2,"Unknown command. Type help.",0x0C);
            }

            if(row>21){
                text_screen();
                row=15;
            }
            put(++row,2,"> ",0x0F);
            n=0;
        }else if(sc==0x0E){
            if(n>0){
                n--;
                VGA_TEXT[row*COLS+4+n]=(u16)' '|((u16)0x07<<8);
            }
        }else{
            char c=keychar(sc);
            if(c && n<60){
                cmd[n++]=c;
                VGA_TEXT[row*COLS+4+n-1]=(u16)c|((u16)0x0F<<8);
            }
        }
    }
}

void kmain(void){
    pit_init();
    shell();
    for(;;) __asm__ volatile("hlt");
}
