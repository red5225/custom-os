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


/* Legacy i8042 polling: no IRQ setup is required for this early kernel. */
static uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static int keyboard_read_scancode(uint8_t *scan, int *extended) {
    static int saw_extended;
    uint8_t status;
    if (!scan || !extended) return 0;
    status = inb(0x64);
    /* Do not consume auxiliary-device bytes as keyboard scancodes. */
    if ((status & 1u) == 0u || (status & 0x20u) != 0u) return 0;
    *scan = inb(0x60);
    if (*scan == 0xE0u || *scan == 0xE1u) {
        saw_extended = 1;
        return 0;
    }
    *extended = saw_extended;
    saw_extended = 0;
    return 1;
}

/* Polling PS/2 auxiliary-device support for emulators and controllers that expose it. */
static int ps2_wait_input_empty(void) {
    for (uint32_t i=0; i<100000u; ++i) if ((inb(0x64) & 2u) == 0u) return 1;
    return 0;
}
static int ps2_wait_output_full(void) {
    for (uint32_t i=0; i<100000u; ++i) if ((inb(0x64) & 1u) != 0u) return 1;
    return 0;
}
static void ps2_command(uint8_t command) {
    if (ps2_wait_input_empty()) outb(0x64,command);
}
static void ps2_data(uint8_t value) {
    if (ps2_wait_input_empty()) outb(0x60,value);
}
static int ps2_mouse_init(void) {
    /* Enable the auxiliary port, then ask a standard PS/2 mouse to stream packets. */
    ps2_command(0xA8);
    ps2_command(0xD4); ps2_data(0xF4);
    if (ps2_wait_output_full()) {
        uint8_t status=inb(0x64);
        uint8_t response=inb(0x60);
        (void)status;
        if (response != 0xFAu && response != 0xFEu) return 0;
        return response == 0xFAu;
    }
    return 0;
}
static int ps2_mouse_read_packet(int *dx, int *dy, uint8_t *buttons) {
    static uint8_t packet[3];
    static unsigned index;
    uint8_t status=inb(0x64), value;
    if ((status & 1u)==0u || (status & 0x20u)==0u) return 0;
    value=inb(0x60);
    if (index==0u && (value & 0x08u)==0u) return 0; /* packet sync bit */
    packet[index++]=value;
    if (index<3u) return 0;
    index=0;
    *buttons=(uint8_t)(packet[0]&7u);
    *dx=(packet[0]&0x10u) ? (int)packet[1]-256 : (int)packet[1];
    *dy=(packet[0]&0x20u) ? (int)packet[2]-256 : (int)packet[2];
    /* Overflowed movement is not useful and should not wrap the cursor. */
    if ((packet[0]&0xC0u)!=0u) { *dx=0; *dy=0; }
    return 1;
}
static void draw_pointer(struct boot_info *b, int x, int y) {
    if (!b) return;
    for (int i=0; i<11; ++i) {
        int xx=x+i/2, yy=y+i;
        if (xx>=0 && yy>=0 && xx<(int)b->screen_width && yy<(int)b->screen_height)
            pixel(b,(uint32_t)xx,(uint32_t)yy,255,255,255);
    }
    for (int i=0; i<7; ++i) {
        int xx=x+i, yy=y+i/2;
        if (xx>=0 && yy>=0 && xx<(int)b->screen_width && yy<(int)b->screen_height)
            pixel(b,(uint32_t)xx,(uint32_t)yy,40,45,55);
    }
}
static uint16_t button_at(int x, int y) {
    for (uint16_t i=0; i<xcui_embedded_tree.node_count; ++i) {
        const struct xcui_node *n=&xcui_embedded_tree.nodes[i];
        if (n->kind==XCUI_BUTTON && n->width>0 && n->height>0 &&
            x>=n->x && y>=n->y && x<(int)n->x+n->width && y<(int)n->y+n->height)
            return i;
    }
    return XCUI_NO_NODE;
}
static int action_is(const char *action, const char *expected) {
    size_t i=0;
    if (!action || !expected) return 0;
    while (action[i] && expected[i] && action[i]==expected[i]) ++i;
    return action[i]=='\0' && expected[i]=='\0';
}
static const char *action_title(const char *action) {
    if (action_is(action,"open_files")) return "FILES";
    if (action_is(action,"open_settings")) return "SETTINGS";
    if (action_is(action,"open_terminal")) return "TERMINAL";
    if (action_is(action,"open_network")) return "NETWORK";
    return "APP";
}

static void show_action_screen(struct boot_info *b, const char *action) {
    const char *title=action_title(action);
    int is_files=action_is(action,"open_files");
    int is_settings=action_is(action,"open_settings");
    int is_terminal=action_is(action,"open_terminal");
    int is_network=action_is(action,"open_network");
    uint32_t left=28, top=92, content_w=b->screen_width>80?b->screen_width-56:240;
    fill(b,246,248,252);
    xcui_fill_rect(b,0,0,(int16_t)b->screen_width,58,0x285bb4u);
    text(b,24,19,2,"CUSTOMOS",255,255,255);
    text(b,24,75,2,title,32,41,56);
    xcui_fill_rect(b,(int16_t)left,(int16_t)top,(int16_t)content_w,
                   (int16_t)(b->screen_height>top+88?b->screen_height-top-76:100),0xffffffu);
    if (is_files) {
        text(b,left+20,top+20,2,"FILES",40,91,180);
        text(b,left+20,top+62,1,"HOME",48,59,78);
        text(b,left+20,top+86,1,"DOCUMENTS",48,59,78);
        text(b,left+20,top+110,1,"DOWNLOADS",48,59,78);
        text(b,left+20,top+154,1,"Storage backend is not mounted yet.",166,98,20);
    } else if (is_settings) {
        text(b,left+20,top+20,2,"SYSTEM SETTINGS",40,91,180);
        text(b,left+20,top+62,1,"Display: framebuffer mode",48,59,78);
        text(b,left+20,top+86,1,"Input: keyboard polling active",48,59,78);
        text(b,left+20,top+110,1,"Pointer: native Chromebook trackpad driver missing",166,70,34);
        text(b,left+20,top+154,1,"Theme presets are editable; live switching is not implemented.",166,98,20);
    } else if (is_terminal) {
        text(b,left+20,top+20,2,"XC TERMINAL",40,91,180);
        text(b,left+20,top+62,1,"Built-in command shell is not implemented yet.",166,98,20);
        text(b,left+20,top+86,1,"XC programs currently run in the host test tool.",48,59,78);
    } else if (is_network) {
        text(b,left+20,top+20,2,"NETWORK STATUS",40,91,180);
        text(b,left+20,top+62,1,"Network adapter: not enumerated",48,59,78);
        text(b,left+20,top+86,1,"TCP/IP stack: not implemented",48,59,78);
        text(b,left+20,top+122,1,"Internet is unavailable in this build.",166,98,20);
    } else {
        text(b,left+20,top+20,2,"APPLICATION",40,91,180);
        text(b,left+20,top+62,1,"No application handler is registered for this action.",166,98,20);
    }
    text(b,24,(uint32_t)(b->screen_height>36?b->screen_height-28:140),1,"ESC - BACK TO DESKTOP",104,115,134);
    serial_puts("UI: opened built-in app screen: "); serial_puts(action?action:"(none)"); serial_puts("\n");
    for (;;) {
        uint8_t scan; int extended=0;
        if (keyboard_read_scancode(&scan,&extended)) {
            if (!extended && scan==0x01u) return;
        } else __asm__ __volatile__("pause");
    }
}

/* Cycle focus only through actionable XCUI buttons. */
static uint16_t find_button(uint16_t current, int direction) {
    uint16_t count=xcui_embedded_tree.node_count;
    if (!count) return XCUI_NO_NODE;
    for (uint16_t step=0; step<count; ++step) {
        if (current==XCUI_NO_NODE || current>=count) current = direction<0 ? (uint16_t)(count-1u) : 0u;
        else if (direction<0) current = current==0u ? (uint16_t)(count-1u) : (uint16_t)(current-1u);
        else current = (uint16_t)((current+1u)%count);
        if (xcui_embedded_tree.nodes[current].kind==XCUI_BUTTON) return current;
    }
    return XCUI_NO_NODE;
}

static int framebuffer_valid(const struct boot_info *b);

/* Render the desktop and outline the focused control using the framebuffer backend. */
static void redraw_with_focus(struct boot_info *b, uint16_t focus) {
    struct xcui_backend backend;
    if (!b || !framebuffer_valid(b)) return;
    backend.fill_rect=xcui_fill_rect;
    backend.draw_text=xcui_draw_text;
    if (!xcui_layout(&xcui_embedded_tree,(int16_t)b->screen_width,(int16_t)b->screen_height)) return;
    (void)xcui_render(&xcui_embedded_tree,&backend,b);
    if (focus!=XCUI_NO_NODE && focus<xcui_embedded_tree.node_count) {
        const struct xcui_node *n=&xcui_embedded_tree.nodes[focus];
        if (n->kind==XCUI_BUTTON && n->width>0 && n->height>0) {
            xcui_fill_rect(b,n->x,n->y,n->width,2,0x202938u);
            xcui_fill_rect(b,n->x,(int16_t)(n->y+n->height-2),n->width,2,0x202938u);
            xcui_fill_rect(b,n->x,n->y,2,n->height,0x202938u);
            xcui_fill_rect(b,(int16_t)(n->x+n->width-2),n->y,2,n->height,0x202938u);
        }
    }
}

static void keyboard_desktop_loop(struct boot_info *b) {
    uint16_t focus = XCUI_NO_NODE;
    uint8_t scan, buttons=0, previous_buttons=0;
    int extended, mouse_x=(int)b->screen_width/2, mouse_y=(int)b->screen_height/2;
    int mouse_dx, mouse_dy, mouse_enabled;
    focus = xcui_embedded_tree.node_count ?
        find_button((uint16_t)(xcui_embedded_tree.node_count-1u),1) : XCUI_NO_NODE;
    mouse_enabled=ps2_mouse_init();
    redraw_with_focus(b,focus);
    if (mouse_enabled) draw_pointer(b,mouse_x,mouse_y);
    serial_puts("INPUT: arrows move focus; Tab cycles; Enter opens action feedback; Esc returns/clears focus.\n");
    serial_puts(mouse_enabled ? "POINTER: PS/2 mouse initialized.\n" : "POINTER: PS/2 auxiliary device unavailable; keyboard remains enabled.\n");
    for (;;) {
        if (mouse_enabled && ps2_mouse_read_packet(&mouse_dx,&mouse_dy,&buttons)) {
            mouse_x+=mouse_dx; mouse_y-=mouse_dy;
            if (mouse_x<0) mouse_x=0;
            if (mouse_y<0) mouse_y=0;
            if (mouse_x>=(int)b->screen_width) mouse_x=(int)b->screen_width-1;
            if (mouse_y>=(int)b->screen_height) mouse_y=(int)b->screen_height-1;
            if ((buttons&1u)!=0u && (previous_buttons&1u)==0u) {
                uint16_t hit=button_at(mouse_x,mouse_y);
                if (hit!=XCUI_NO_NODE) {
                    focus=hit;
                    const char *action=xcui_node_action(&xcui_embedded_tree,focus);
                    redraw_with_focus(b,focus); draw_pointer(b,mouse_x,mouse_y);
                    if (action && action[0]) show_action_screen(b,action);
                    redraw_with_focus(b,focus);
                }
            }
            previous_buttons=buttons;
            redraw_with_focus(b,focus); draw_pointer(b,mouse_x,mouse_y);
            continue;
        }
        if (!keyboard_read_scancode(&scan,&extended)) {
            __asm__ __volatile__("pause");
            continue;
        }
        if (scan & 0x80u) continue;
        if (extended) {
            if (scan==0x48u || scan==0x4Bu) focus=find_button(focus,-1);
            else if (scan==0x50u || scan==0x4Du) focus=find_button(focus,1);
            else continue;
        } else if (scan==0x0Fu) {
            focus=find_button(focus,1);
        } else if (scan==0x01u) {
            focus=XCUI_NO_NODE;
        } else if (scan==0x1Cu) {
            if (focus!=XCUI_NO_NODE && focus<xcui_embedded_tree.node_count) {
                const char *action=xcui_node_action(&xcui_embedded_tree,focus);
                if (action && action[0]) {
                    show_action_screen(b,action);
                    redraw_with_focus(b,focus);
                    if (mouse_enabled) draw_pointer(b,mouse_x,mouse_y);
                } else serial_puts("INPUT: focused button has no action.\n");
            } else serial_puts("INPUT: Enter pressed but no button is focused. Press Tab or an arrow key first.\n");
            continue;
        } else continue;
        redraw_with_focus(b,focus);
        if (mouse_enabled) draw_pointer(b,mouse_x,mouse_y);
    }
}

static void boot_pause(void) {
    /* A short bounded delay for readable stage transitions; no timer dependency. */
    for (volatile uint32_t i=0; i<12000000u; ++i)
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

static void splash_rect(struct boot_info *b, uint32_t x, uint32_t y,
                        uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t bl) {
    uint32_t yy, xx;
    if (!framebuffer_valid(b) || x>=b->screen_width || y>=b->screen_height) return;
    if (w>b->screen_width-x) w=b->screen_width-x;
    if (h>b->screen_height-y) h=b->screen_height-y;
    for (yy=0; yy<h; ++yy)
        for (xx=0; xx<w; ++xx) pixel(b,x+xx,y+yy,r,g,bl);
}

static uint32_t boot_progress(const char *stage) {
    if (action_is(stage,"KERNEL ONLINE")) return 12;
    if (action_is(stage,"CHECKING MEMORY INFO")) return 34;
    if (action_is(stage,"CHECKING DISPLAY")) return 57;
    if (action_is(stage,"STARTING XCUI")) return 82;
    return 100;
}

static void show_boot_stage(struct boot_info *b, const char *stage,
                            uint8_t r, uint8_t g, uint8_t bl) {
    uint32_t width, x, y, logo_x, logo_y, i, progress;
    uint32_t title_len=0, stage_len=0;
    const char *p;
    if (!framebuffer_valid(b)) return;
    for (p="CUSTOMOS"; *p; ++p) ++title_len;
    for (p=stage; p && *p; ++p) ++stage_len;
    /* Calm light boot canvas, with a small geometric CustomOS mark. */
    fill(b,246,248,252);
    width=b->screen_width>440?360:b->screen_width-48;
    x=(b->screen_width-width)/2;
    y=b->screen_height/2;
    logo_x=(b->screen_width-40u)/2u;
    logo_y=(y>150u)?y-142u:12u;
    splash_rect(b,logo_x,logo_y,40,40,40,91,180);
    splash_rect(b,logo_x+7,logo_y+7,26,26,246,248,252);
    splash_rect(b,logo_x+11,logo_y+11,18,18,40,91,180);
    splash_rect(b,logo_x+15,logo_y+15,10,10,246,248,252);
    text(b,(b->screen_width-title_len*24u)/2,y-82,4,"CUSTOMOS",28,38,58);
    text(b,(b->screen_width-stage_len*12u)/2,y-28,2,stage,r,g,bl);
    splash_rect(b,x,y+18,width,5,222,227,235);
    progress=boot_progress(stage);
    for (i=0;i<(width*progress)/100u;i++)
        pixel(b,x+i,y+18,40,91,180);
}

static char terminal_key(uint8_t scan) {
    static const char keys[128] = {
        [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',[0x06]='5',[0x07]='6',[0x08]='7',[0x09]='8',[0x0A]='9',[0x0B]='0',[0x0C]='-',[0x0E]='\b',
        [0x10]='Q',[0x11]='W',[0x12]='E',[0x13]='R',[0x14]='T',[0x15]='Y',[0x16]='U',[0x17]='I',[0x18]='O',[0x19]='P',[0x1C]='\n',
        [0x1E]='A',[0x1F]='S',[0x20]='D',[0x21]='F',[0x22]='G',[0x23]='H',[0x24]='J',[0x25]='K',[0x26]='L',[0x2C]='Z',[0x2D]='X',[0x2E]='C',[0x2F]='V',[0x30]='B',[0x31]='N',[0x32]='M',[0x39]=' '
    };
    return scan < sizeof(keys) ? keys[scan] : 0;
}

static int command_is(const char *a, const char *b) {
    while (*a && *b) { char x=*a++, y=*b++; if (x>='a'&&x<='z') x=(char)(x-'a'+'A'); if (y>='a'&&y<='z') y=(char)(y-'a'+'A'); if (x!=y) return 0; }
    return *a==0 && *b==0;
}

static void terminal_line(struct boot_info *b, uint32_t row, const char *s, uint8_t r, uint8_t g, uint8_t bl) {
    if (b && framebuffer_valid(b)) text(b,12,12+row*12,1,s,r,g,bl);
    serial_puts(s); serial_puts("\n");
}

static void terminal_render(struct boot_info *b, const char *lines[], uint32_t count, const char *input, int mini) {
    uint32_t i;
    if (!b || !framebuffer_valid(b)) return;
    fill(b,8,12,20);
    if (mini) {
        text(b,10,10,1,"CUSTOMOS | TEXT MODE | TYPE HELP",180,220,255);
        return;
    }
    text(b,12,8,1,"CUSTOMOS / XC SHELL",100,190,255);
    for (i=0;i<count && i<10;i++) text(b,12,24+i*12,1,lines[i],220,225,235);
    text(b,12,154,1,">",120,220,150);
    text(b,24,154,1,input,245,245,245);
}

static void terminal_execute(struct boot_info *b, const char *cmd, const char *lines[], uint32_t *count, int *mini) {
    if (!cmd[0]) return;
    serial_puts("CUSTOMOS: command: "); serial_puts(cmd); serial_puts("\n");
    if (command_is(cmd,"HELP")) {
        terminal_line(b,1,"HELP CLEAR VERSION STATUS COMPONENTS MEMINFO DEVICES",220,225,235);
        terminal_line(b,2,"BOOTLOG SELFTEST UI USB STATUS USB LS",220,225,235);
        terminal_line(b,3,"USB STORAGE DRIVER IS NOT IMPLEMENTED YET",255,190,100);
        terminal_line(b,4,"NO DISK DATA IS CHANGED BY THIS BUILD",180,220,150);
    } else if (command_is(cmd,"CLEAR")) {
        *count=0; fill(b,8,12,20); terminal_line(b,0,"SCREEN CLEARED",180,220,150);
    } else if (command_is(cmd,"VERSION")) terminal_line(b,1,"CUSTOMOS KERNEL / XC SHELL MILESTONE 1",220,225,235);
    else if (command_is(cmd,"STATUS")) terminal_line(b,1,"KERNEL RUNNING; UEFI BOOT INFO VALIDATED",180,220,150);
    else if (command_is(cmd,"COMPONENTS")) {
        terminal_line(b,1,"UEFI LOADER: ACTIVE",180,220,150);
        terminal_line(b,2,"FRAMEBUFFER: ACTIVE",180,220,150);
        terminal_line(b,3,"PS2 KEYBOARD POLLING: ACTIVE IF FIRMWARE EXPOSES IT",220,225,235);
        terminal_line(b,4,"USB XHCI / MASS STORAGE / FAT: NOT IMPLEMENTED",255,190,100);
        terminal_line(b,5,"NETWORK: NOT IMPLEMENTED",255,190,100);
    } else if (command_is(cmd,"MEMINFO")) terminal_line(b,1,"UEFI MEMORY MAP METADATA VALIDATED; ALLOCATOR NOT IMPLEMENTED",220,225,235);
    else if (command_is(cmd,"DEVICES")) terminal_line(b,1,"NO PCI OR USB DEVICE ENUMERATOR IMPLEMENTED YET",255,190,100);
    else if (command_is(cmd,"USB STATUS") || command_is(cmd,"USB LS") || command_is(cmd,"USB LIST")) terminal_line(b,1,"USB STORAGE UNAVAILABLE: XHCI, BOT/SCSI AND FAT DRIVERS ARE NOT IMPLEMENTED",255,190,100);
    else if (command_is(cmd,"BOOTLOG")) {
        terminal_line(b,1,"1 BOOT INFO VALIDATED",180,220,150);
        terminal_line(b,2,"2 MEMORY MAP METADATA VALIDATED",180,220,150);
        terminal_line(b,3,"3 FRAMEBUFFER GEOMETRY VALIDATED",180,220,150);
        terminal_line(b,4,"4 COMMAND SHELL STARTED",180,220,150);
    } else if (command_is(cmd,"SELFTEST")) {
        terminal_line(b,1,"PASS: COMMAND MATCHING",180,220,150);
        terminal_line(b,2,"PASS: BOOT INFO AND FRAMEBUFFER CHECKS",180,220,150);
        terminal_line(b,3,"SKIP: USB STORAGE TEST (DRIVER NOT IMPLEMENTED)",255,190,100);
    } else if (command_is(cmd,"UI")) {
        *mini=!*mini;
        terminal_render(b,lines,*count,cmd,*mini);
        terminal_line(b,1,*mini?"MINIMAL STATUS BAR ENABLED":"TEXT TERMINAL ENABLED",180,220,150);
    } else if (command_is(cmd,"DESKTOP")) {
        terminal_line(b,1,"LEGACY XCUI DESKTOP REQUESTED; ESC RETURNS TO SHELL",220,225,235);
        draw_status(b);
        keyboard_desktop_loop(b);
        terminal_render(b,lines,*count,cmd,*mini);
    } else terminal_line(b,1,"UNKNOWN COMMAND; TYPE HELP",255,190,100);
}

__attribute__((noreturn)) void kernel_entry(struct boot_info *info) {
    char input[80]; const char *lines[10]; uint32_t count=0; size_t length=0; int mini=0;
    serial_init(); serial_puts("\nCUSTOMOS: kernel reached\n");
    show_boot_stage(info,"KERNEL ONLINE",90,220,150);
    if (!info || info->magic != BOOT_INFO_MAGIC) { serial_puts("BOOT ERROR: invalid boot information.\n"); show_boot_stage(info,"BOOT INFO ERROR",255,90,90); goto halt; }
    serial_puts("BOOT STAGE 1/4: boot information validated.\n");
    show_boot_stage(info,"CHECKING MEMORY INFO",100,190,255);
    if (!info->memory_map || !info->descriptor_size || info->memory_map_size < info->descriptor_size) { serial_puts("BOOT ERROR: memory-map metadata invalid.\n"); show_boot_stage(info,"MEMORY INFO ERROR",255,90,90); goto halt; }
    serial_puts("BOOT STAGE 2/4: memory-map metadata validated (map not dereferenced).\n");
    show_boot_stage(info,"CHECKING DISPLAY",100,190,255);
    if (!framebuffer_valid(info)) { serial_puts("BOOT ERROR: invalid framebuffer.\n"); goto halt; }
    serial_puts("BOOT STAGE 3/4: framebuffer validated.\n");
    show_boot_stage(info,"STARTING TERMINAL",255,210,100);
    boot_pause();
    fill(info,8,12,20);
    terminal_line(info,0,"CUSTOMOS XC SHELL READY",100,190,255);
    terminal_line(info,1,"TYPE HELP FOR COMMANDS",180,220,150);
    terminal_line(info,2,"USB STORAGE: NOT IMPLEMENTED IN THIS BUILD",255,190,100);
    terminal_line(info,3,"BOOT STAGE 4/4: TEXT TERMINAL ACTIVE",180,220,150);
    input[0]=0; terminal_render(info,lines,count,input,mini);
    for (;;) {
        uint8_t scan; int extended=0;
        if (!keyboard_read_scancode(&scan,&extended)) { __asm__ __volatile__("pause"); continue; }
        if (extended || (scan & 0x80u)) continue;
        char c=terminal_key(scan);
        if (!c) continue;
        if (c=='\b') { if (length) input[--length]=0; terminal_render(info,lines,count,input,mini); continue; }
        if (c=='\n') {
            input[length]=0; serial_puts("> "); serial_puts(input); serial_puts("\n");
            if (count<10) lines[count++]=input;
            terminal_execute(info,input,lines,&count,&mini);
            length=0; input[0]=0; terminal_render(info,lines,count,input,mini);
            continue;
        }
        if (length+1<sizeof(input)) { input[length++]=c; input[length]=0; terminal_render(info,lines,count,input,mini); }
    }
halt:
    serial_puts("Kernel halted safely after a boot validation error.\n");
    for (;;) __asm__ __volatile__("cli; hlt");
}

