#include <stdint.h>
#include <stddef.h>
#include "xcui.h"

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

#define BOOT_INFO_MAGIC UINT64_C(0x435553544F534231)
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

extern struct xcui_tree xcui_embedded_tree;

static void xcui_fill_rect(void *context, int16_t x, int16_t y,
                           int16_t width, int16_t height, uint32_t color) {
    struct boot_info *b = (struct boot_info *)context;
    int32_t x0=x, y0=y, x1=(int32_t)x+width, y1=(int32_t)y+height;
    uint8_t r=(uint8_t)(color>>16), g=(uint8_t)(color>>8), bl=(uint8_t)color;
    if (!b || width<=0 || height<=0) return;
    if (x0<0) x0=0;
    if (y0<0) y0=0;
    if (x1>(int32_t)b->screen_width) x1=(int32_t)b->screen_width;
    if (y1>(int32_t)b->screen_height) y1=(int32_t)b->screen_height;
    for (int32_t yy=y0; yy<y1; ++yy)
        for (int32_t xx=x0; xx<x1; ++xx)
            pixel(b,(uint32_t)xx,(uint32_t)yy,r,g,bl);
}

static void xcui_draw_text(void *context, int16_t x, int16_t y,
                           const char *value, uint16_t size, uint32_t color) {
    char uppercase[XCUI_TEXT_CAP];
    size_t i=0;
    unsigned scale = size >= 21 ? 3u : (size >= 14 ? 2u : 1u);
    while (value && value[i] && i+1<sizeof(uppercase)) {
        char c=value[i];
        if (c>='a' && c<='z') c=(char)(c-'a'+'A');
        uppercase[i++]=c;
    }
    uppercase[i]='\0';
    text((struct boot_info *)context,(uint32_t)(x<0?0:x),(uint32_t)(y<0?0:y),
         scale,uppercase,(uint8_t)(color>>16),(uint8_t)(color>>8),(uint8_t)color);
}

static void draw_status(struct boot_info *b) {
    struct xcui_backend backend;
    if (!b || !b->framebuffer_base || b->screen_width < 320 || b->screen_height < 160 ||
        b->screen_width > 32767 || b->screen_height > 32767 ||
        (b->pixel_format != PIXEL_RGB_RESERVED && b->pixel_format != PIXEL_BGR_RESERVED)) return;
    backend.fill_rect=xcui_fill_rect;
    backend.draw_text=xcui_draw_text;
    if (xcui_layout(&xcui_embedded_tree,(int16_t)b->screen_width,(int16_t)b->screen_height) &&
        xcui_render(&xcui_embedded_tree,&backend,b)) {
        serial_puts("XCUI: embedded CustomOS desktop rendered.\n");
        return;
    }
    serial_puts("XCUI: render failed; drawing fallback status screen.\n");
    fill(b,8,14,28);
    text(b,b->screen_width/12,b->screen_height/5,4,"CUSTOMOS",235,245,255);
    text(b,b->screen_width/12,b->screen_height/5+60,2,"XCUI ERROR",255,110,100);
}
static void boot_pause(void) {
    /* A short bounded delay for readable stage transitions; no timer dependency. */
    for (volatile uint32_t i=0; i<1800000u; ++i)
        __asm__ __volatile__("pause");
}

static int framebuffer_valid(const struct boot_info *b) {
    if (!b || !b->framebuffer_base) return 0;
    if (b->screen_width < 320 || b->screen_height < 160 ||
        b->screen_width > 32767 || b->screen_height > 32767) return 0;
    if (b->pixels_per_scanline < b->screen_width ||
        b->pixels_per_scanline > 32767) return 0;
    if (b->pixel_format != PIXEL_RGB_RESERVED &&
        b->pixel_format != PIXEL_BGR_RESERVED) return 0;
    if ((uint64_t)b->pixels_per_scanline * (uint64_t)b->screen_height >
        UINT64_MAX / 4u) return 0;
    return 1;
}

static void show_boot_stage(struct boot_info *b, const char *stage,
                            uint8_t r, uint8_t g, uint8_t bl) {
    if (!framebuffer_valid(b)) return;
    fill(b,8,14,28);
    text(b,b->screen_width/12,b->screen_height/5,4,"CUSTOMOS",235,245,255);
    text(b,b->screen_width/12,b->screen_height/5+60,2,stage,r,g,bl);
}

__attribute__((noreturn)) void kernel_entry(struct boot_info *info) {
    serial_init();
    serial_puts("\nCUSTOMOS: kernel reached\n");
    show_boot_stage(info,"KERNEL ONLINE",90,220,150);
    boot_pause();

    if (!info || info->magic != BOOT_INFO_MAGIC) {
        serial_puts("BOOT ERROR: invalid or missing boot information; desktop skipped.\n");
        show_boot_stage(info,"BOOT INFO ERROR",255,90,90);
        goto halt;
    }
    serial_puts("BOOT STAGE 1/4: boot information validated.\n");
    show_boot_stage(info,"CHECKING MEMORY INFO",100,190,255);
    boot_pause();
    if (!info->memory_map || !info->descriptor_size ||
        info->memory_map_size < info->descriptor_size) {
        serial_puts("BOOT ERROR: memory-map metadata invalid; desktop skipped.\n");
        show_boot_stage(info,"MEMORY INFO ERROR",255,90,90);
        goto halt;
    }
    serial_puts("BOOT STAGE 2/4: memory-map metadata validated (map not dereferenced).\n");
    show_boot_stage(info,"CHECKING DISPLAY",100,190,255);
    boot_pause();
    if (!framebuffer_valid(info)) {
        serial_puts("BOOT ERROR: unsupported or invalid framebuffer; desktop skipped.\n");
        goto halt;
    }
    serial_puts("BOOT STAGE 3/4: framebuffer geometry, stride and pixel format validated.\n");
    show_boot_stage(info,"STARTING XCUI",255,210,100);
    boot_pause();
    serial_puts("BOOT STAGE 4/4: launching XCUI renderer.\n");
    draw_status(info);
    serial_puts("BOOT COMPLETE: kernel reached safe idle state.\n");
halt:
    serial_puts("Kernel halted safely; no keyboard/mouse event loop is implemented yet.\n");
    for (;;) __asm__ __volatile__("cli; hlt");
}
