#include "fb.h"
#include "kernel.h"
#include "serial.h"

typedef struct {
    uint32_t type;
    uint32_t size;
} __attribute__((packed)) mb2_tag_t;

static fb_info_t FB;
static int FB_READY = 0;

// 8x8 bitmap font for ASCII 32..127 (partial: minimal set for demo)
extern const uint8_t font8x8_basic[128][8];

static inline uint32_t make_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (0xFFu<<24) | (r<<16) | (g<<8) | b;
}

int fb_is_available(void) { return FB_READY; }
uint32_t fb_width(void){ return FB.width; }
uint32_t fb_height(void){ return FB.height; }

// Page tables provided by boot.s for dynamic mapping
extern uint32_t page_directory[];
extern uint32_t page_table_fb0[];
extern uint32_t page_table_fb1[];

// Map the framebuffer physical memory into a fixed higher-half virtual window
// to avoid colliding with kernel higher-half mapping (PDE[768]).
#define KFB_VA   0xE0000000u
#define KFB_PDE  (KFB_VA >> 22)
static uint32_t kfb_virt_base = KFB_VA;

static void map_framebuffer_window(uint32_t phys_addr) {
    // Map 8MB window starting at KFB_VA to framebuffer's physical 8MB region
    uint32_t base4m = phys_addr & 0xFFC00000u; // align down to 4MB
    // Install page tables into directory at KFB_PDE and KFB_PDE+1
    page_directory[KFB_PDE]     = ((uint32_t)page_table_fb0) | 0x003; // present|rw
    page_directory[KFB_PDE + 1] = ((uint32_t)page_table_fb1) | 0x003;
    // Fill entries: first 4MB
    for (uint32_t i=0;i<1024;i++) page_table_fb0[i] = (base4m + i*0x1000) | 0x003;
    // Next 4MB
    for (uint32_t i=0;i<1024;i++) page_table_fb1[i] = (base4m + 0x400000 + i*0x1000) | 0x003;
    // Flush TLB by reloading CR3
    uint32_t cr3; __asm__ __volatile__("mov %%cr3, %0" : "=r"(cr3));
    __asm__ __volatile__("mov %0, %%cr3" :: "r"(cr3));
    // Adjust FB.addr so callers use the virtual window + offset within 8MB
    uint32_t offset_in_8m = phys_addr - base4m;
    FB.addr = kfb_virt_base + offset_in_8m;
}

int fb_init_from_multiboot(uint32_t mb2_info) {
    if (!mb2_info) return 0;
    uint8_t* p = (uint8_t*)mb2_info;
    uint32_t total_size = *(uint32_t*)p; (void)total_size; p += 8; // skip size + reserved
    while (1) {
        mb2_tag_t* tag = (mb2_tag_t*)p;
        if (tag->type == 0) break; // end
        if (tag->type == 8) { // framebuffer
            // Multiboot2 framebuffer tag layout
            uint64_t addr = *(uint64_t*)(p + 8);
            uint32_t pitch = *(uint32_t*)(p + 16);
            uint32_t width = *(uint32_t*)(p + 20);
            uint32_t height = *(uint32_t*)(p + 24);
            uint8_t bpp = *(uint8_t*)(p + 28);
            uint8_t type = *(uint8_t*)(p + 29);
                if (type != 1) {
                    serial_write("[fb] non-linear fb type unsupported: ");
                    serial_write_dec(type);
                    serial_write("\r\n");
                    return 0;
                }
            FB.addr = (uint32_t)addr;
            FB.pitch = pitch;
            FB.width = width;
            FB.height = height;
            FB.bpp = bpp;
            FB.type = type;
            if (type == 1) {
                FB.red_pos   = *(uint8_t*)(p + 32);
                FB.red_mask  = *(uint8_t*)(p + 33); // actually size
                FB.green_pos = *(uint8_t*)(p + 34);
                FB.green_mask= *(uint8_t*)(p + 35); // size
                FB.blue_pos  = *(uint8_t*)(p + 36);
                FB.blue_mask = *(uint8_t*)(p + 37); // size
            }
            // Map framebuffer memory into fixed higher-half window and set FB.addr to virtual
            map_framebuffer_window(FB.addr);
            FB_READY = 1;
            serial_write("[fb] mode "); serial_write_dec(width); serial_write("x"); serial_write_dec(height); serial_write("x"); serial_write_dec(bpp); serial_write(" pitch="); serial_write_dec(pitch);
            serial_write(" type="); serial_write_dec(type);
            serial_write(" R("); serial_write_dec(FB.red_pos); serial_write(","); serial_write_dec(FB.red_mask);
            serial_write(") G("); serial_write_dec(FB.green_pos); serial_write(","); serial_write_dec(FB.green_mask);
            serial_write(") B("); serial_write_dec(FB.blue_pos); serial_write(","); serial_write_dec(FB.blue_mask);
            serial_write(")\r\n");
            return 1;
        }
        // advance to next tag (8 byte aligned)
        uint32_t size = tag->size; p += ((size + 7) & ~7);
    }
    return 0;
}

static inline uint32_t pack_color(uint32_t argb) {
    if (FB.type == 1) {
        // Use positions and sizes (sizes stored in *_mask fields)
        uint32_t r = (argb >> 16) & 0xFF;
        uint32_t g = (argb >> 8)  & 0xFF;
        uint32_t b = (argb >> 0)  & 0xFF;
        // scale down to channel sizes
        if (FB.red_mask   < 8) r >>= (8 - FB.red_mask);
        if (FB.green_mask < 8) g >>= (8 - FB.green_mask);
        if (FB.blue_mask  < 8) b >>= (8 - FB.blue_mask);
        return (r << FB.red_pos) | (g << FB.green_pos) | (b << FB.blue_pos);
    }
    // Fallback assume x8r8g8b8
    return argb;
}

static inline void fb_store(uint32_t x, uint32_t y, uint32_t argb) {
    if (!FB_READY) return;
    if (x >= FB.width || y >= FB.height) return;
    uint8_t* base = (uint8_t*)(uintptr_t)FB.addr;
    uint32_t bpp = FB.bpp;
    uint32_t off = y * FB.pitch + x * (bpp/8);
    uint32_t pix = pack_color(argb);
    if (bpp == 32) {
        *(uint32_t*)(base + off) = pix;
    } else if (bpp == 24) {
        // Little endian: write lowest 3 bytes
        base[off+0] = (uint8_t)(pix & 0xFF);
        base[off+1] = (uint8_t)((pix >> 8) & 0xFF);
        base[off+2] = (uint8_t)((pix >> 16) & 0xFF);
    } else if (bpp == 16) {
        *(uint16_t*)(base + off) = (uint16_t)pix;
    } else {
        // Unsupported bpp; no-op
    }
}

void fb_fill(uint32_t argb) {
    if (!FB_READY) return;
    for (uint32_t y = 0; y < FB.height; y++) {
        for (uint32_t x = 0; x < FB.width; x++) fb_store(x, y, argb);
    }
}

void fb_putpixel(uint32_t x, uint32_t y, uint32_t argb) { fb_store(x,y,argb); }

void fb_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t argb) {
    for (uint32_t j=0;j<h;j++) for (uint32_t i=0;i<w;i++) fb_store(x+i,y+j,argb);
}

// CP437 extended line-drawing glyphs (subset)
static const uint8_t GLYPH_C4_SINGLE_H[8] = {0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00};      // 'â' 0xC4
static const uint8_t GLYPH_CD_DOUBLE_H[8] = {0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00};      // 'â' 0xCD
static const uint8_t GLYPH_B3_SINGLE_V[8] = {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18};      // 'â' 0xB3
static const uint8_t GLYPH_BA_DOUBLE_V[8] = {0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C};      // 'â' 0xBA
// Corners (approximate; we may refine later)
static const uint8_t GLYPH_DA_CORNER_TL[8] = {0x00,0x00,0x00,0x1F,0x18,0x18,0x18,0x18};     // 'â' 0xDA
static const uint8_t GLYPH_BF_CORNER_TR[8] = {0x00,0x00,0x00,0xF8,0x18,0x18,0x18,0x18};     // 'â' 0xBF
static const uint8_t GLYPH_C0_CORNER_BL[8] = {0x18,0x18,0x18,0x18,0x1F,0x00,0x00,0x00};     // 'â' 0xC0
static const uint8_t GLYPH_D9_CORNER_BR[8] = {0x18,0x18,0x18,0x18,0xF8,0x00,0x00,0x00};     // 'â' 0xD9
static const uint8_t GLYPH_C9_CORNER_TL_D[8] = {0x00,0x00,0x00,0x3F,0x3C,0x3C,0x3C,0x3C};   // 'â' 0xC9
static const uint8_t GLYPH_BB_CORNER_TR_D[8] = {0x00,0x00,0x00,0xFC,0x3C,0x3C,0x3C,0x3C};   // 'â' 0xBB
static const uint8_t GLYPH_C8_CORNER_BL_D[8] = {0x3C,0x3C,0x3C,0x3C,0x3F,0x00,0x00,0x00};   // 'â' 0xC8
static const uint8_t GLYPH_BC_CORNER_BR_D[8] = {0x3C,0x3C,0x3C,0x3C,0xFC,0x00,0x00,0x00};   // 'â' 0xBC

static inline const uint8_t* fb_get_glyph(unsigned char uc) {
    if (uc < 128) return font8x8_basic[uc];
    switch (uc) {
        case 0xC4: return GLYPH_C4_SINGLE_H; // 'â'
        case 0xCD: return GLYPH_CD_DOUBLE_H; // 'â'
        case 0xB3: return GLYPH_B3_SINGLE_V; // 'â'
        case 0xBA: return GLYPH_BA_DOUBLE_V; // 'â'
        case 0xDA: return GLYPH_DA_CORNER_TL; // 'â'
        case 0xBF: return GLYPH_BF_CORNER_TR; // 'â'
        case 0xC0: return GLYPH_C0_CORNER_BL; // 'â'
        case 0xD9: return GLYPH_D9_CORNER_BR; // 'â'
        case 0xC9: return GLYPH_C9_CORNER_TL_D; // 'â'
        case 0xBB: return GLYPH_BB_CORNER_TR_D; // 'â'
        case 0xC8: return GLYPH_C8_CORNER_BL_D; // 'â'
        case 0xBC: return GLYPH_BC_CORNER_BR_D; // 'â'
        default: return 0;
    }
}

void fb_blit_glyph(uint32_t x, uint32_t y, char ch, uint32_t fg, uint32_t bg) {
    unsigned char uc = (unsigned char)ch;
    const uint8_t* g = fb_get_glyph(uc);
    if (!g) {
        for (int row=0; row<8; row++) for (int col=0; col<8; col++) fb_store(x+col, y+row, bg);
        return;
    }
    for (int row=0; row<8; row++) {
        uint8_t bits = g[row];
        for (int col=0; col<8; col++) {
            int mask = 0x80 >> col; // MSB-first, left-to-right
            fb_store(x+col, y+row, (bits & mask) ? fg : bg);
        }
    }
}

void fb_blit_glyph_scaled(uint32_t x, uint32_t y, char ch, uint32_t fg, uint32_t bg, uint32_t scale) {
    if (scale == 0) scale = 1;
    unsigned char uc = (unsigned char)ch;
    const uint8_t* g = fb_get_glyph(uc);
    if (!g) {
        for (uint32_t row=0; row<8*scale; row++) 
            for (uint32_t col=0; col<8*scale; col++) 
                fb_store(x+col, y+row, bg);
        return;
    }
    
    // Clean character rendering without artifacts
    for (int row=0; row<8; row++) {
        uint8_t bits = g[row];
        for (int col=0; col<8; col++) {
            int mask = 0x80 >> col;
            uint32_t color = (bits & mask) ? fg : bg;
            
            // Draw clean scale x scale pixel blocks
            for (uint32_t sy=0; sy<scale; sy++) {
                for (uint32_t sx=0; sx<scale; sx++) {
                    fb_store(x + col*scale + sx, y + row*scale + sy, color);
                }
            }
        }
    }
}

void fb_draw_string(uint32_t x, uint32_t y, const char* s, uint32_t fg, uint32_t bg) {
    while (*s) { fb_blit_glyph(x, y, *s, fg, bg); x += 8; s++; }
}

void fb_draw_string_scaled(uint32_t x, uint32_t y, const char* s, uint32_t fg, uint32_t bg, uint32_t scale) {
    if (scale == 0) scale = 1;
    while (*s) { 
        fb_blit_glyph_scaled(x, y, *s, fg, bg, scale); 
        x += 8 * scale; 
        s++; 
    }
}

int fb_copy_out32(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t* dst) {
    if (!FB_READY || FB.bpp != 32 || FB.type != 1) return 0;
    if (x+w > FB.width || y+h > FB.height) return 0;
    uint8_t* base = (uint8_t*)(uintptr_t)FB.addr;
    for (uint32_t j=0; j<h; j++) {
        uint32_t off = (y+j) * FB.pitch + x * 4;
        for (uint32_t i=0; i<w; i++) {
            dst[j*w + i] = *(uint32_t*)(base + off + i*4);
        }
    }
    return 1;
}

int fb_copy_in32(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t* src) {
    if (!FB_READY || FB.bpp != 32 || FB.type != 1) return 0;
    if (x+w > FB.width || y+h > FB.height) return 0;
    uint8_t* base = (uint8_t*)(uintptr_t)FB.addr;
    for (uint32_t j=0; j<h; j++) {
        uint32_t off = (y+j) * FB.pitch + x * 4;
        for (uint32_t i=0; i<w; i++) {
            *(uint32_t*)(base + off + i*4) = src[j*w + i];
        }
    }
    return 1;
}

int fb_blit_argb32(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t* src, uint32_t srcStridePx) {
    if (!FB_READY) return 0;
    if (x+w > FB.width || y+h > FB.height) return 0;
    uint8_t* base = (uint8_t*)(uintptr_t)FB.addr;
    for (uint32_t j=0; j<h; j++) {
        uint32_t dst_off = (y+j) * FB.pitch + x * (FB.bpp/8);
        const uint32_t* row = src + j*srcStridePx;
        if (FB.bpp == 32) {
            for (uint32_t i=0; i<w; i++) {
                uint32_t pix = pack_color(row[i]);
                *(uint32_t*)(base + dst_off + i*4) = pix;
            }
        } else if (FB.bpp == 24) {
            for (uint32_t i=0; i<w; i++) {
                uint32_t pix = pack_color(row[i]);
                base[dst_off + i*3 + 0] = (uint8_t)(pix & 0xFF);
                base[dst_off + i*3 + 1] = (uint8_t)((pix >> 8) & 0xFF);
                base[dst_off + i*3 + 2] = (uint8_t)((pix >> 16) & 0xFF);
            }
        } else if (FB.bpp == 16) {
            for (uint32_t i=0; i<w; i++) {
                uint32_t pix = pack_color(row[i]);
                *(uint16_t*)(base + dst_off + i*2) = (uint16_t)pix;
            }
        } else {
            return 0;
        }
    }
    return 1;
}

int fb_copy_rect(uint32_t srcx, uint32_t srcy, uint32_t w, uint32_t h, uint32_t dstx, uint32_t dsty) {
    if (!FB_READY) return 0;
    if (!w || !h) return 1;
    if (srcx + w > FB.width || srcy + h > FB.height) return 0;
    if (dstx + w > FB.width || dsty + h > FB.height) return 0;
    uint8_t* base = (uint8_t*)(uintptr_t)FB.addr;
    uint32_t bppB = FB.bpp / 8; if (!bppB) return 0;
    // Decide copy order for overlap safety (like memmove)
    int forward = 1;
    if (dsty > srcy || (dsty == srcy && dstx > srcx)) forward = 0; // copy bottom-up/right-to-left
    if (forward) {
        for (uint32_t j=0; j<h; j++) {
            uint32_t so = (srcy + j) * FB.pitch + srcx * bppB;
            uint32_t doff = (dsty + j) * FB.pitch + dstx * bppB;
            // If horizontal overlap and dst>src, copy right-to-left
            if (dsty == srcy && dstx > srcx) {
                for (int32_t i=(int32_t)w-1; i>=0; --i) {
                    for (uint32_t b=0;b<bppB;b++) base[doff + i*bppB + b] = base[so + i*bppB + b];
                }
            } else {
                for (uint32_t i=0; i<w; i++) {
                    for (uint32_t b=0;b<bppB;b++) base[doff + i*bppB + b] = base[so + i*bppB + b];
                }
            }
        }
    } else {
        for (int32_t j=(int32_t)h-1; j>=0; --j) {
            uint32_t so = (srcy + (uint32_t)j) * FB.pitch + srcx * bppB;
            uint32_t doff = (dsty + (uint32_t)j) * FB.pitch + dstx * bppB;
            if (dsty == srcy && dstx > srcx) {
                for (int32_t i=(int32_t)w-1; i>=0; --i) {
                    for (uint32_t b=0;b<bppB;b++) base[doff + i*bppB + b] = base[so + i*bppB + b];
                }
            } else {
                for (uint32_t i=0; i<w; i++) {
                    for (uint32_t b=0;b<bppB;b++) base[doff + i*bppB + b] = base[so + i*bppB + b];
                }
            }
        }
    }
    return 1;
}

// Tiny font (only ASCII 0..127); for brevity most rows are zeroed; extend as needed
const uint8_t font8x8_basic[128][8] = {
    [32] = {0,0,0,0,0,0,0,0}, // space
    ['+']={0x00,0x10,0x10,0x7C,0x10,0x10,0x00,0x00},
    ['-']={0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0x00},
    ['|']={0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
    ['.']={0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00},
    ['A']={0x18,0x24,0x42,0x7E,0x42,0x42,0,0},
    ['B']={0x7C,0x42,0x7C,0x42,0x42,0x7C,0,0},
    ['C']={0x3C,0x42,0x40,0x40,0x42,0x3C,0,0},
    ['D']={0x78,0x44,0x42,0x42,0x44,0x78,0,0},
    ['E']={0x7E,0x40,0x7C,0x40,0x40,0x7E,0,0},
    ['F']={0x7E,0x40,0x7C,0x40,0x40,0x40,0,0},
    ['G']={0x3C,0x42,0x40,0x4E,0x42,0x3C,0,0},
    ['H']={0x42,0x42,0x7E,0x42,0x42,0x42,0,0},
    ['I']={0x7E,0x18,0x18,0x18,0x18,0x7E,0,0},
    ['J']={0x0E,0x02,0x02,0x02,0x42,0x3C,0,0},
    ['K']={0x42,0x44,0x48,0x70,0x48,0x44,0x42,0},
    ['L']={0x40,0x40,0x40,0x40,0x40,0x7E,0,0},
    ['M']={0x42,0x66,0x5A,0x5A,0x42,0x42,0,0},
    ['N']={0x42,0x62,0x52,0x4A,0x46,0x42,0,0},
    ['O']={0x3C,0x42,0x42,0x42,0x42,0x3C,0,0},
    ['P']={0x7C,0x42,0x42,0x7C,0x40,0x40,0,0},
    ['Q']={0x3C,0x42,0x42,0x4A,0x44,0x3A,0,0},
    ['R']={0x7C,0x42,0x42,0x7C,0x44,0x42,0,0},
    ['S']={0x3C,0x42,0x40,0x3C,0x02,0x42,0x3C,0},
    ['T']={0x7E,0x18,0x18,0x18,0x18,0x18,0,0},
    ['U']={0x42,0x42,0x42,0x42,0x42,0x3C,0,0},
    ['V']={0x42,0x42,0x42,0x42,0x24,0x18,0,0},
    ['W']={0x42,0x42,0x42,0x5A,0x5A,0x66,0x42,0x42},
    ['X']={0x42,0x24,0x18,0x18,0x24,0x42,0,0},
    ['Y']={0x42,0x42,0x24,0x18,0x18,0x18,0,0},
    ['Z']={0x7E,0x04,0x08,0x10,0x20,0x7E,0,0},
    ['a']={0x00,0x00,0x3C,0x02,0x3E,0x42,0x3E,0},
    ['b']={0x40,0x40,0x7C,0x42,0x42,0x7C,0,0},
    ['c']={0x00,0x00,0x3C,0x40,0x40,0x3C,0,0},
    ['d']={0x02,0x02,0x3E,0x42,0x42,0x3E,0,0},
    ['e']={0x00,0x00,0x3C,0x42,0x7E,0x40,0x3E,0},
    ['f']={0x0C,0x10,0x3C,0x10,0x10,0x10,0,0},
    ['g']={0x00,0x00,0x3E,0x42,0x3E,0x02,0x3C,0},
    ['h']={0x40,0x40,0x7C,0x42,0x42,0x42,0,0},
    ['i']={0x00,0x10,0x00,0x30,0x10,0x10,0x38,0},
    ['j']={0x00,0x04,0x00,0x0C,0x04,0x44,0x38,0},
    ['k']={0x40,0x40,0x44,0x48,0x70,0x48,0x44,0},
    ['l']={0x30,0x10,0x10,0x10,0x10,0x38,0,0},
    ['m']={0x00,0x00,0x66,0x7E,0x5A,0x5A,0x5A,0},
    ['n']={0x00,0x00,0x7C,0x42,0x42,0x42,0,0},
    ['o']={0x00,0x00,0x3C,0x42,0x42,0x3C,0,0},
    ['p']={0x00,0x00,0x7C,0x42,0x7C,0x40,0x40,0},
    ['q']={0x00,0x00,0x3E,0x42,0x3E,0x02,0x02,0},
    ['r']={0x00,0x00,0x5C,0x62,0x40,0x40,0,0},
    ['s']={0x00,0x00,0x3E,0x40,0x3C,0x02,0x7C,0},
    ['t']={0x20,0x20,0x7C,0x20,0x20,0x1C,0,0},
    ['u']={0x00,0x00,0x42,0x42,0x42,0x3E,0,0},
    ['v']={0x00,0x00,0x42,0x42,0x24,0x18,0x00,0},
    ['w']={0x00,0x00,0x42,0x5A,0x5A,0x24,0,0},
    ['x']={0x00,0x00,0x42,0x24,0x18,0x24,0x42,0},
    ['y']={0x00,0x00,0x42,0x42,0x3E,0x02,0x3C,0},
    ['z']={0x00,0x00,0x7E,0x04,0x18,0x20,0x7E,0},
    ['0']={0x3C,0x46,0x4A,0x52,0x62,0x3C,0,0},
    ['1']={0x08,0x18,0x08,0x08,0x08,0x1C,0,0},
    ['2']={0x3C,0x42,0x04,0x18,0x20,0x7E,0,0},
    ['3']={0x7E,0x04,0x18,0x04,0x42,0x3C,0,0},
    ['4']={0x04,0x0C,0x14,0x24,0x7E,0x04,0,0},
    ['5']={0x7E,0x40,0x7C,0x02,0x42,0x3C,0,0},
    ['6']={0x1C,0x20,0x7C,0x42,0x42,0x3C,0,0},
    ['7']={0x7E,0x02,0x04,0x08,0x10,0x10,0,0},
    ['8']={0x3C,0x42,0x3C,0x42,0x42,0x3C,0,0},
    ['9']={0x3C,0x42,0x42,0x3E,0x02,0x1C,0,0},
};
