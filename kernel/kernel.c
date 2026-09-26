typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

enum { W = 80, H = 25 };
static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static void putc_at(int x, int y, char c, uint8_t cc) {
    if (x >= 0 && x < W && y >= 0 && y < H)
        VGA[y * W + x] = (uint16_t)(unsigned char)c | ((uint16_t)cc << 8);
}

static void text_at(int x, int y, const char *s, uint8_t cc) {
    while (*s && x < W) putc_at(x++, y, *s++, cc);
}

static void fill(int x, int y, int w, int h, char c, uint8_t cc) {
    for (int yy = y; yy < y + h && yy < H; yy++)
        for (int xx = x; xx < x + w && xx < W; xx++)
            putc_at(xx, yy, c, cc);
}

static void box(int x, int y, int w, int h, uint8_t cc) {
    for (int i = 0; i < w; i++) {
        putc_at(x+i,y,'-',cc); putc_at(x+i,y+h-1,'-',cc);
    }
    for (int i = 0; i < h; i++) {
        putc_at(x,y+i,'|',cc); putc_at(x+w-1,y+i,'|',cc);
    }
    putc_at(x,y,'+',cc); putc_at(x+w-1,y,'+',cc);
    putc_at(x,y+h-1,'+',cc); putc_at(x+w-1,y+h-1,'+',cc);
}

void kmain(unsigned long magic, unsigned long multiboot_info) {
    (void)multiboot_info;

    fill(0,0,W,H,' ',0x17);

    /* Top menu bar */
    fill(0,0,W,2,' ',0x70);
    text_at(2,0,"custom-os",0x7F);
    text_at(18,0,"File",0x7F);
    text_at(24,0,"Edit",0x7F);
    text_at(30,0,"View",0x7F);
    text_at(37,0,"Window",0x7F);
    text_at(46,0,"Help",0x7F);
    text_at(67,0,"WiFi",0x7F);
    text_at(73,0,"100%",0x7F);

    /* Main window */
    fill(8,4,64,14,' ',0x17);
    box(8,4,64,14,0x71);
    fill(9,5,62,2,' ',0x70);
    putc_at(12,6,'o',0x74);
    putc_at(15,6,'o',0x76);
    putc_at(18,6,'o',0x72);
    text_at(31,6,"About custom-os",0x7F);

    text_at(13,9,"CUSTOM-OS",0x7B);
    text_at(13,10,"A tiny 32-bit desktop kernel",0x7F);
    text_at(13,12,"Status",0x70);
    text_at(30,12,"ONLINE",0x7A);
    text_at(13,13,"Architecture",0x70);
    text_at(30,13,"x86 / 32-bit",0x7F);
    text_at(13,14,"Boot",0x70);
    text_at(30,14,magic==0x2BADB002UL?"MULTIBOOT OK":"MULTIBOOT ERROR",0x7A);
    text_at(13,16,"Built for UTM",0x7F);

    /* Dock */
    fill(21,20,38,4,' ',0x70);
    box(21,20,38,4,0x71);
    text_at(25,21,"[Finder]",0x7B);
    text_at(36,21,"[Term]",0x7A);
    text_at(46,21,"[Info]",0x7E);
    text_at(54,21,"[OS]",0x7F);

    for (;;) __asm__ volatile ("hlt");
}
