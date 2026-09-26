typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

enum { VGA_WIDTH = 80, VGA_HEIGHT = 25 };
static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static uint32_t *fb;
static uint32_t fb_pitch;
static uint32_t fb_width;
static uint32_t fb_height;
static uint8_t fb_bpp;
static uint8_t red_pos, green_pos, blue_pos;

static void vga_clear(void) {
    for (int y = 0; y < VGA_HEIGHT; y++)
        for (int x = 0; x < VGA_WIDTH; x++)
            VGA[y * VGA_WIDTH + x] = (uint16_t)' ' | (uint16_t)(0x07 << 8);
}

static void vga_print(const char *s) {
    static int pos = 0;
    while (*s && pos < VGA_WIDTH * VGA_HEIGHT) {
        char c = *s++;
        if (c == '\n') {
            pos += VGA_WIDTH - (pos % VGA_WIDTH);
            continue;
        }
        VGA[pos++] = (uint16_t)c | (uint16_t)(0x07 << 8);
    }
}

static void fb_put_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (!fb || x < 0 || y < 0 || (uint32_t)x >= fb_width || (uint32_t)y >= fb_height)
        return;

    uint32_t pixel = ((uint32_t)r << red_pos) |
                     ((uint32_t)g << green_pos) |
                     ((uint32_t)b << blue_pos);
    fb[y * (fb_pitch / 4) + x] = pixel;
}

static void fb_clear(uint8_t r, uint8_t g, uint8_t b) {
    if (!fb || fb_bpp != 32) return;
    for (uint32_t y = 0; y < fb_height; y++)
        for (uint32_t x = 0; x < fb_width; x++)
            fb_put_pixel((int)x, (int)y, r, g, b);
}

static const uint8_t glyph_C[8] = {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0};
static const uint8_t glyph_U[8] = {0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0};
static const uint8_t glyph_S[8] = {0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0};
static const uint8_t glyph_T[8] = {0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0};
static const uint8_t glyph_O[8] = {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0};
static const uint8_t glyph_M[8] = {0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0};
static const uint8_t glyph_X[8] = {0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0};
static const uint8_t glyph_B[8] = {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0};
static const uint8_t glyph_E[8] = {0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0};
static const uint8_t glyph_D[8] = {0x7C,0x66,0x66,0x66,0x66,0x66,0x7C,0};
static const uint8_t glyph_I[8] = {0x7E,0x18,0x18,0x18,0x18,0x18,0x7E,0};
static const uint8_t glyph_P[8] = {0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0};
static const uint8_t glyph_L[8] = {0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0};
static const uint8_t glyph_N[8] = {0x66,0x76,0x7E,0x6E,0x66,0x66,0x66,0};
static const uint8_t glyph_Y[8] = {0x66,0x66,0x3C,0x18,0x18,0x18,0x18,0};
static const uint8_t glyph_F[8] = {0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0};

static const uint8_t *glyph(char c) {
    switch (c) {
        case 'C': return glyph_C; case 'U': return glyph_U;
        case 'S': return glyph_S; case 'T': return glyph_T;
        case 'O': return glyph_O; case 'M': return glyph_M;
        case 'X': return glyph_X; case 'B': return glyph_B;
        case 'E': return glyph_E; case 'D': return glyph_D;
        case 'I': return glyph_I; case 'P': return glyph_P;
        case 'L': return glyph_L; case 'N': return glyph_N;
        case 'Y': return glyph_Y; case 'F': return glyph_F;
        default: return 0;
    }
}

static void fb_text(const char *s, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (!fb || fb_bpp != 32) return;
    while (*s) {
        char c = *s++;
        if (c == ' ') { x += 10; continue; }
        const uint8_t *gph = glyph(c);
        if (!gph) { x += 10; continue; }
        for (int row = 0; row < 8; row++)
            for (int col = 0; col < 8; col++)
                if (gph[row] & (uint8_t)(1u << (7 - col)))
                    fb_put_pixel(x + col, y + row, r, g, b);
        x += 10;
    }
}

static void framebuffer_init(unsigned long multiboot_info) {
    uint32_t *info = (uint32_t *)multiboot_info;
    if (!info || !(info[0] & (1u << 12))) return;

    uint32_t low = info[22];
    uint32_t high = info[23];
    if (high != 0) return;

    fb = (uint32_t *)(unsigned long)low;
    fb_pitch = info[24];
    fb_width = info[25];
    fb_height = info[26];

    uint8_t *bytes = (uint8_t *)info;
    fb_bpp = bytes[108];
    if (bytes[109] != 1 || fb_bpp != 32) {
        fb = 0;
        return;
    }

    red_pos = bytes[112];
    green_pos = bytes[114];
    blue_pos = bytes[116];
}

void kmain(unsigned long magic, unsigned long multiboot_info) {
    (void)magic;

    vga_clear();
    vga_print("custom-os starting...\n");

    framebuffer_init(multiboot_info);

    if (fb) {
        fb_clear(8, 12, 20);
        fb_text("CUSTOM OS", 80, 80, 80, 220, 255);
        fb_text("DISPLAY ONLINE", 80, 110, 255, 255, 255);
        fb_text("32 BIT X86", 80, 140, 180, 255, 180);
    } else {
        vga_print("framebuffer unavailable; using VGA text.\n");
    }

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
