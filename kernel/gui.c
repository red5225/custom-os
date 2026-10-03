#include "gui.h"
#include "fb.h"
#include "mouse.h"
#include "serial.h"

static int win_x = 100, win_y = 80, win_w = 360, win_h = 200;
static int dragging = 0; static int drag_dx = 0, drag_dy = 0;
static int last_mx = -1, last_my = -1; static unsigned char last_btn = 0xFF;
static int last_win_x = -9999, last_win_y = -9999;
static int cur_x = -1, cur_y = -1; static int cur_saved = 0;
static uint32_t cur_back[8*8];

// Backbuffer (ARGB32) and dirty rect tracking
static uint32_t* bb = 0; static uint32_t bb_w = 0, bb_h = 0;
typedef struct { int x1,y1,x2,y2; } rect_t;
static rect_t dirty = {0,0,0,0};
static inline void dirty_reset(void){ dirty.x1=0x7FFFFFFF; dirty.y1=0x7FFFFFFF; dirty.x2=-0x7FFFFFFF; dirty.y2=-0x7FFFFFFF; }
static inline void dirty_add(int x, int y, int w, int h){
    if (w<=0 || h<=0) return;
    if (dirty.x1 > x) dirty.x1 = x;
    if (dirty.y1 > y) dirty.y1 = y;
    if (dirty.x2 < x + w) dirty.x2 = x + w;
    if (dirty.y2 < y + h) dirty.y2 = y + h;
}
static inline int dirty_has(void){ return dirty.x1<dirty.x2 && dirty.y1<dirty.y2; }
static inline void dirty_get(rect_t* r){ *r = dirty; }
// 60Hz throttle with 100Hz PIT
static int tick_accum = 0; // add 3 per 100Hz tick, flush when >=5 (~60Hz)

static void draw_cursor(int x, int y) {
    // 8x8 arrow cursor (white with black outline)
    static const uint8_t cur_bits[8] = {
        0b10000000,
        0b11000000,
        0b11100000,
        0b11110000,
        0b11111000,
        0b11100000,
        0b11000000,
        0b10000000,
    };
    for (int j=0;j<8;j++) {
        uint8_t b = cur_bits[j];
        for (int i=0;i<8;i++) {
            uint32_t c = (b & (0x80 >> i)) ? 0xFFFFFFFF : 0x00000000;
            if (c) fb_putpixel(x+i, y+j, c);
        }
    }
}

// draw_window integrated into draw_frame (backbuffer)

static void draw_background_gradient(void) {
    // Vertical gradient background using true color (32-bit fixed-point)
    uint32_t h = bb_h; uint32_t w = bb_w;
    int r0=24, g0=48, b0=64;
    int r1=56, g1=24, b1=72;
    int steps = (h > 1) ? ((int)h - 1) : 1;
    int r_fp = r0 << 16, g_fp = g0 << 16, b_fp = b0 << 16;
    int dr_fp = ((r1 - r0) << 16) / steps;
    int dg_fp = ((g1 - g0) << 16) / steps;
    int db_fp = ((b1 - b0) << 16) / steps;
    for (uint32_t y=0; y<h; y++) {
        uint8_t r = (uint8_t)(r_fp >> 16);
        uint8_t g = (uint8_t)(g_fp >> 16);
        uint8_t b = (uint8_t)(b_fp >> 16);
        uint32_t color = 0xFF000000u | (r<<16) | (g<<8) | b;
        uint32_t* row = bb + y*bb_w;
        for (uint32_t x=0; x<w; x++) row[x] = color;
        r_fp += dr_fp; g_fp += dg_fp; b_fp += db_fp;
    }
}

static void draw_frame(int cx, int cy) {
    (void)cx; (void)cy; // Suppress unused parameter warnings
    draw_background_gradient();
    // Drop shadow
    for (int j=2;j<win_h+2;j++) for (int i=2;i<win_w+2;i++) {
        int px = win_x + i, py = win_y + j; if (px<0||py<0||px>=(int)bb_w||py>=(int)bb_h) continue;
        uint32_t base = bb[py*bb_w + px];
        // darken base a bit for shadow
        uint8_t r=(base>>16)&0xFF,g=(base>>8)&0xFF,b=(base)&0xFF;
        r = (uint8_t)(r*3/4); g=(uint8_t)(g*3/4); b=(uint8_t)(b*3/4);
        bb[py*bb_w + px] = 0xFF000000|(r<<16)|(g<<8)|b;
    }
    // Window body
    for (int j=0;j<win_h;j++) for (int i=0;i<win_w;i++) {
        int px = win_x + i, py = win_y + j; if (px<0||py<0||px>=(int)bb_w||py>=(int)bb_h) continue;
        bb[py*bb_w + px] = 0xFF202830;
    }
    // Border
    uint32_t border = 0xFF506070;
    for (int i=0;i<win_w;i++) {
        int x1=win_x+i, y1=win_y, y2=win_y+win_h-1; if (x1>=0&&x1<(int)bb_w){ if(y1>=0&&y1<(int)bb_h) bb[y1*bb_w+x1]=border; if(y2>=0&&y2<(int)bb_h) bb[y2*bb_w+x1]=border; }
    }
    for (int j=0;j<win_h;j++) {
        int y1=win_y+j, x1=win_x, x2=win_x+win_w-1; if (y1>=0&&y1<(int)bb_h){ if(x1>=0&&x1<(int)bb_w) bb[y1*bb_w+x1]=border; if(x2>=0&&x2<(int)bb_w) bb[y1*bb_w+x2]=border; }
    }
    // Title bar
    for (int j=0;j<18;j++) for (int i=1;i<win_w-1;i++) {
        int px = win_x + i, py = win_y + j; if (px<0||py<0||px>=(int)bb_w||py>=(int)bb_h) continue;
        bb[py*bb_w + px] = 0xFF3A5A9A;
    }
    // Title text (draw via fb glyph into backbuffer by composing pixels)
    const char* t = "Helixa Window"; int tx = win_x + 6, ty = win_y + 4; char ch;
    while ((ch = *t++)) {
        // 8x8 glyph using existing function; fetch pixels by reading fb API is awkward, so re-render directly to backbuffer
        extern const uint8_t font8x8_basic[128][8];
        const uint8_t* g = font8x8_basic[(unsigned char)ch];
        for (int row=0; row<8; row++) {
            uint8_t bits = g[row];
            for (int col=0; col<8; col++) {
                int px = tx+col, py = ty+row; if (px<0||py<0||px>=(int)bb_w||py>=(int)bb_h) continue;
                // Standard left-to-right bit order: MSB at col 7
                int mask = 0x80 >> col;
                bb[py*bb_w + px] = (bits & mask) ? 0xFFFFFFFF : 0xFF3A5A9A;
            }
        }
        tx += 8;
    }
    // Cursor drawn later directly to FB over blit
    // Only mark the window region dirty
    dirty_add(win_x, win_y, win_w, win_h);
}

static void clamp_cursor(int* x, int* y) {
    int w = (int)fb_width();
    int h = (int)fb_height();
    if (*x < 0) *x = 0;
    if (*y < 0) *y = 0;
    if (*x > w - 8) *x = w - 8;
    if (*y > h - 8) *y = h - 8;
}

static void cursor_restore(void) {
    if (!cur_saved) return;
    fb_copy_in32((uint32_t)cur_x, (uint32_t)cur_y, 8, 8, cur_back);
    cur_saved = 0;
}

static void cursor_save_at(int x, int y) {
    clamp_cursor(&x, &y);
    fb_copy_out32((uint32_t)x, (uint32_t)y, 8, 8, cur_back);
    cur_x = x; cur_y = y; cur_saved = 1;
}

void gui_init(void) {
    // Draw first frame once so users see something even before timer starts
    // Allocate backbuffer
    bb_w = fb_width(); bb_h = fb_height();
    // naive single static allocation; in a kernel you might have a heap, here we use a static pointer via a simple bump (not available), so rely on a fixed array size is not possible.
    // For demo, use first-call static from BSS via a simple wrapper in fb.câhere we assume we can use a static array by asking for width*height*4 via a simple kmalloc. Since we don't have one, fallback: reuse cursor buffer technique isn't sufficient.
    // Instead, allocate from a static max; to keep changes small, we use a simple trick: reuse fb_copy_out32 into a statically sized area if resolution <= 1280x1024.
    static uint32_t static_bb[(1024*768)];
    bb = static_bb;
    // Initialize frame
    int ix = win_x + 10, iy = win_y + 30; clamp_cursor(&ix, &iy);
    draw_frame(ix, iy);
    // Flush full backbuffer to FB once
    serial_write("[gui] blit backbuffer...\r\n");
    if (!fb_blit_argb32(0,0, bb_w, bb_h, bb, bb_w)) {
        // Fallback: draw minimally using direct FB ops
        serial_write("[gui] blit failed, fallback fb_fill/fb_rect\r\n");
        // Simple dark background
        fb_fill(0xFF202020);
        // Window background and title
        fb_rect(win_x, win_y, win_w, win_h, 0xFF202830);
        fb_rect(win_x, win_y, win_w, 18, 0xFF405A88);
        fb_draw_string(win_x + 6, win_y + 4, "Helixa Window", 0xFFFFFFFF, 0xFF405A88);
    }
    serial_write("[gui] first frame drawn\r\n");
    cursor_save_at(ix, iy);
    dirty_reset();
}

void gui_tick(void) {
    int x, y; unsigned char btn; mouse_get(&x, &y, &btn);
    clamp_cursor(&x, &y);
    // Drag logic on title bar
    if ((btn & 0x1) && !dragging) {
        if (x >= win_x && x < win_x + win_w && y >= win_y && y < win_y + 18) {
            dragging = 1; drag_dx = x - win_x; drag_dy = y - win_y;
        }
    } else if (!(btn & 0x1)) dragging = 0;
    if (dragging) { win_x = x - drag_dx; win_y = y - drag_dy; if (win_x<0) win_x=0; if (win_y<0) win_y=0; }

    // Redraw strategy:
    // - If window moved (dragging), redraw full frame once.
    // - Else, only update cursor: restore old area, then draw cursor at new pos.
    int window_moved = (win_x != last_win_x || win_y != last_win_y);
    int cursor_moved = (x != last_mx || y != last_my);
    if (window_moved) { draw_frame(x, y); }

    // 60Hz throttle using integer accumulator: +3 per 100Hz tick, flush when >=5
    tick_accum += 3;
    if (tick_accum >= 5 && dirty_has()) {
        serial_write("[gui] blit dirty\r\n");
        rect_t r; dirty_get(&r);
        if (r.x1<0) r.x1=0;
        if (r.y1<0) r.y1=0;
        if (r.x2>(int)bb_w) r.x2=(int)bb_w;
        if (r.y2>(int)bb_h) r.y2=(int)bb_h;
        uint32_t w = (uint32_t)(r.x2 - r.x1), h = (uint32_t)(r.y2 - r.y1);
        if (!fb_blit_argb32((uint32_t)r.x1, (uint32_t)r.y1, w, h, bb + r.y1*bb_w + r.x1, bb_w)) {
            serial_write("[gui] blit failed\r\n");
        }
        dirty_reset();
        // Cursor will be redrawn on top below
        cur_saved = 0;
        tick_accum -= 5;
    }

    // Cursor update always after blit
    if (cursor_moved || window_moved) {
        cursor_restore();
        cursor_save_at(x, y);
        draw_cursor(x, y);
    }
    last_mx = x; last_my = y; last_btn = btn; last_win_x = win_x; last_win_y = win_y;
}
