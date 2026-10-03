#include "fb_console.h"
#include "fb.h"
#include "serial.h"

static uint32_t FGC = 0xFFFFFFFF, BGC = 0xFF000000;
static int cx = 0, cy = 0; // in glyph cells

void fbcon_init(uint32_t fg, uint32_t bg) { FGC = fg; BGC = bg; cx = cy = 0; }
void fbcon_clear(void) { fb_fill(BGC); cx = cy = 0; }

void fbcon_putc(char c) {
    if (!fb_is_available()) return;
    if (c == '\n') { cy++; cx = 0; return; }
    if (c == '\r') { cx = 0; return; }
    int px = cx * 8, py = cy * 8;
    if (px + 8 >=  (int)0x7FFFFFFF) return; // guard
    fb_blit_glyph(px, py, c, FGC, BGC);
    cx++;
    if ((uint32_t)(cx*8) >= fb_width()) { cx = 0; cy++; }
}

void fbcon_write(const char* s) { while (*s) fbcon_putc(*s++); }

void fb_show_welcome_screen(void) {
    if (!fb_is_available()) return;
    serial_write("[fb] welcome draw\r\n");
    uint32_t W = fb_width(), H = fb_height();
    // Background (blue-ish), border (yellow), bars (white), text colors
    uint32_t bg = 0xFF104090; // blue
    uint32_t bar = 0xFFFFFFFF; // white bars
    uint32_t border = 0xFFE0C040; // yellow-ish
    uint32_t title_fg = 0xFFFFFFFF;
    uint32_t status_fg = 0xFFE0C040;
    // Fill background
    fb_rect(0,0,W,H,bg);
    // Top and bottom bars (approx rows)
    fb_rect(0, H/12, W, H/48, bar);
    fb_rect(0, H - H/12 - H/48, W, H/48, bar);
    // Border
    fb_rect(0,0,W,2,border); fb_rect(0,H-2,W,2,border); fb_rect(0,0,2,H,border); fb_rect(W-2,0,2,H,border);
    // Centered title
    const char* ttl = " HelixaOS v1.0 ";
    int tlen = 0; for (const char* p=ttl; *p; ++p) tlen++;
    int tw = tlen * 8; int tx = (int)W/2 - tw/2; int ty = (int)H/12 + 4;
    fb_draw_string((uint32_t)tx, (uint32_t)ty, ttl, title_fg, bar);
    // Status (right-ish)
    const char* st = " [Uptime] Framebuffer ";
    int slen = 0; for (const char* p=st; *p; ++p) slen++;
    int sx = (int)W - slen*8 - 16; int sy = (int)H - H/12 - H/48 + 2;
    if (sx < 8) sx = 8;
    fb_draw_string((uint32_t)sx, (uint32_t)sy, st, status_fg, bar);
    // Centered splash box
    int box_w = 46*8, box_h = 7*8;
    int box_x = (int)W/2 - box_w/2;
    int box_y = (int)H/2 - box_h/2;
    uint32_t box_fg = 0xFFB0E0FF, box_bg = bg;
    // Draw box frame and text
    fb_rect((uint32_t)box_x, (uint32_t)box_y, (uint32_t)box_w, 2, box_fg);
    fb_rect((uint32_t)box_x, (uint32_t)(box_y+box_h-2), (uint32_t)box_w, 2, box_fg);
    fb_rect((uint32_t)box_x, (uint32_t)box_y, 2, (uint32_t)box_h, box_fg);
    fb_rect((uint32_t)(box_x+box_w-2), (uint32_t)box_y, 2, (uint32_t)box_h, box_fg);
    const char* l1 = "Welcome to HelixaOS";
    const char* l2 = "Designed for AI";
    int l1w=0,l2w=0; for(const char* p=l1;*p;++p)l1w++; for(const char* p=l2;*p;++p)l2w++;
    int l1x = (int)W/2 - (l1w*8)/2; int l2x = (int)W/2 - (l2w*8)/2;
    fb_draw_string((uint32_t)l1x, (uint32_t)(box_y + 2*8), l1, 0xFFFFFFFF, box_bg);
    fb_draw_string((uint32_t)l2x, (uint32_t)(box_y + 4*8), l2, 0xFFEEEEEE, box_bg);
}
