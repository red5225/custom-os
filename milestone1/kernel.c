#include <stdint.h>
#include <stddef.h>

struct boot_info {
    uint64_t magic;
    uint64_t memory_map_size;
    uint64_t memory_map_key;
    uint64_t descriptor_size;
    uint32_t descriptor_version;
    uint32_t reserved;
    void *memory_map;
    uint64_t framebuffer_base;
    uint32_t screen_width;
    uint32_t screen_height;
    uint32_t pixels_per_scanline;
    uint32_t pixel_format;
};

#define BOOT_INFO_MAGIC UINT64_C(0x4E4F4C424F4F5431)
#define PIXEL_RGB_RESERVED 0u
#define PIXEL_BGR_RESERVED 1u

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

static void serial_init(void) {
    outb(0x3F9, 0x00); outb(0x3FB, 0x80); outb(0x3F8, 0x01);
    outb(0x3F9, 0x00); outb(0x3FB, 0x03); outb(0x3FA, 0xC7); outb(0x3FC, 0x0B);
}

static void serial_puts(const char *s) {
    while (*s) { if (*s == '\n') outb(0x3F8, '\r'); outb(0x3F8, (uint8_t)*s++); }
}

/* Five-by-seven uppercase bitmap font. One bit means a lit pixel. */
static uint8_t glyph_row(char c, unsigned row) {
#define G(ch,a,b,c,d,e,f,g) case ch: { static const uint8_t p[7]={a,b,c,d,e,f,g}; return p[row]; }
    switch (c) {
        G('A',14,17,17,31,17,17,17) G('B',30,17,17,30,17,17,30)
        G('C',14,17,16,16,16,17,14) G('D',30,17,17,17,17,17,30)
        G('E',31,16,16,30,16,16,31) G('F',31,16,16,30,16,16,16)
        G('G',14,17,16,23,17,17,15) G('H',17,17,17,31,17,17,17)
        G('I',14,4,4,4,4,4,14) G('J',7,2,2,2,18,18,12)
        G('K',17,18,20,24,20,18,17) G('L',16,16,16,16,16,16,31)
        G('M',17,27,21,21,17,17,17) G('N',17,25,21,19,17,17,17)
        G('O',14,17,17,17,17,17,14) G('P',30,17,17,30,16,16,16)
        G('Q',14,17,17,17,21,18,13) G('R',30,17,17,30,20,18,17)
        G('S',15,16,16,14,1,1,30) G('T',31,4,4,4,4,4,4)
        G('U',17,17,17,17,17,17,14) G('V',17,17,17,17,17,10,4)
        G('W',17,17,17,21,21,21,10) G('X',17,17,10,4,10,17,17)
        G('Y',17,17,10,4,4,4,4) G('Z',31,1,2,4,8,16,31)
        G('0',14,17,19,21,25,17,14) G('1',4,12,4,4,4,4,14)
        G('2',14,17,1,2,4,8,31) G('3',30,1,1,14,1,1,30)
        G('4',2,6,10,18,31,2,2) G('5',31,16,16,30,1,1,30)
        G('6',14,16,16,30,17,17,14) G('7',31,1,2,4,8,8,8)
        G('8',14,17,17,14,17,17,14) G('9',14,17,17,15,1,1,14)
        G('-',0,0,0,31,0,0,0) G(':',0,4,4,0,4,4,0)
        G('.',0,0,0,0,0,12,12) G('!',4,4,4,4,4,0,4)
        default: return 0;
    }
#undef G
}

static void pixel(struct boot_info *b, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t bl) {
    if (!b || !b->framebuffer_base || x >= b->screen_width || y >= b->screen_height ||
        b->pixels_per_scanline < b->screen_width) return;
    volatile uint8_t *p = (volatile uint8_t *)(uintptr_t)b->framebuffer_base +
        ((uint64_t)y * b->pixels_per_scanline + x) * 4;
    if (b->pixel_format == PIXEL_RGB_RESERVED) { p[0]=r; p[1]=g; p[2]=bl; p[3]=0; }
    else if (b->pixel_format == PIXEL_BGR_RESERVED) { p[0]=bl; p[1]=g; p[2]=r; p[3]=0; }
}

static void fill(struct boot_info *b, uint8_t r, uint8_t g, uint8_t bl) {
    if (!b || !b->framebuffer_base || b->screen_width == 0 || b->screen_height == 0 ||
        (b->pixel_format != PIXEL_RGB_RESERVED && b->pixel_format != PIXEL_BGR_RESERVED)) return;
    for (uint32_t y=0; y<b->screen_height; ++y)
        for (uint32_t x=0; x<b->screen_width; ++x) pixel(b,x,y,r,g,bl);
}

static void text(struct boot_info *b, uint32_t x, uint32_t y, unsigned scale,
                 const char *s, uint8_t r, uint8_t g, uint8_t bl) {
    uint32_t origin=x;
    for (; *s; ++s) {
        if (*s=='\n') { x=origin; y += 9*scale; continue; }
        for (unsigned row=0; row<7; ++row) {
            uint8_t bits=glyph_row(*s,row);
            for (unsigned col=0; col<5; ++col) if (bits & (1u << (4-col)))
                for (unsigned dy=0; dy<scale; ++dy)
                    for (unsigned dx=0; dx<scale; ++dx) pixel(b,x+col*scale+dx,y+row*scale+dy,r,g,bl);
        }
        x += 6*scale;
    }
}

static void draw_status(struct boot_info *b) {
    if (!b || !b->framebuffer_base || b->screen_width < 320 || b->screen_height < 160 ||
        (b->pixel_format != PIXEL_RGB_RESERVED && b->pixel_format != PIXEL_BGR_RESERVED)) return;
    fill(b, 8, 14, 28);
    uint32_t left = b->screen_width / 12;
    uint32_t top = b->screen_height / 5;
    for (uint32_t x=left; x<b->screen_width-left; ++x)
        for (uint32_t t=0; t<3; ++t) pixel(b,x,top+43+t,30,190,235);
    text(b,left,top,5,"NOL OS",235,245,255);
    text(b,left,top+72,3,"KERNEL RUNNING",80,230,160);
    text(b,left,top+116,2,"MILESTONE 1 - NATIVE UEFI",175,195,220);
    text(b,left,top+154,2,"DISPLAY INITIALIZED",175,195,220);
}

__attribute__((noreturn)) void kernel_entry(struct boot_info *info) {
    serial_init();
    serial_puts("\nNOL OS MILESTONE 1: kernel reached\n");
    if (info && info->magic == BOOT_INFO_MAGIC) {
        serial_puts("Boot info: valid; UEFI boot services have ended.\n");
        if (info->framebuffer_base) {
            serial_puts("Framebuffer: GOP available; drawing kernel status screen.\n");
            draw_status(info);
        } else {
            serial_puts("Framebuffer: unavailable; screen output skipped.\n");
        }
    } else {
        serial_puts("Boot info: invalid.\n");
    }
    serial_puts("Kernel halted safely.\n");
    for (;;) __asm__ __volatile__("cli; hlt");
}
