#include <stdint.h>
#include "terminal.h"
#include "font8x8_basic.h"

static volatile uint16_t *vga = (volatile uint16_t *)0xB8000;
static uint32_t *fb;
static uint64_t pitch, width, height;
static uint8_t fb_enabled;
static uint8_t red_shift, green_shift, blue_shift;
static uint8_t red_size, green_size, blue_size;
static uint32_t cursor_x, cursor_y;

static uint32_t channel(uint8_t value, uint8_t size, uint8_t shift) {
    if (!size) return 0;
    uint32_t max = (1u << size) - 1u;
    return ((uint32_t)value * max / 255u) << shift;
}

static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return channel(r, red_size, red_shift) |
           channel(g, green_size, green_shift) |
           channel(b, blue_size, blue_shift);
}

static void fb_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (!fb_enabled || x >= width || y >= height) return;
    ((uint32_t *)((uint8_t *)fb + y * pitch))[x] = color;
}

static void fb_clear_glyph(uint32_t x, uint32_t y) {
    uint32_t bg = rgb(8, 10, 14);
    for (uint32_t yy = 0; yy < 8; ++yy)
        for (uint32_t xx = 0; xx < 8; ++xx)
            fb_pixel(x + xx, y + yy, bg);
}

static void draw_char(char c) {
    unsigned char uc = (unsigned char)c;
    if (uc >= 128) uc = '?';
    uint32_t px = cursor_x * 8;
    uint32_t py = cursor_y * 10;
    uint32_t fg = rgb(232, 238, 245);

    for (uint32_t row = 0; row < 8; ++row) {
        uint8_t bits = (uint8_t)font8x8_basic[uc][row];
        for (uint32_t col = 0; col < 8; ++col)
            if (bits & (1u << col))
                fb_pixel(px + col, py + row, fg);
    }
}

static void fb_newline(void) {
    cursor_x = 0;
    ++cursor_y;
    uint32_t rows = (uint32_t)(height / 10);
    if (cursor_y >= rows) {
        terminal_clear();
    }
}

void terminal_init_fb(const struct limine_framebuffer *f) {
    const uint8_t *raw = (const uint8_t *)f;
    fb = *(uint32_t **)(raw + 0);
    width = *(const uint64_t *)(raw + 8);
    height = *(const uint64_t *)(raw + 16);
    pitch = *(const uint64_t *)(raw + 24);
    uint16_t bpp = *(const uint16_t *)(raw + 32);
    uint8_t model = *(const uint8_t *)(raw + 34);
    red_size = *(const uint8_t *)(raw + 35);
    red_shift = *(const uint8_t *)(raw + 36);
    green_size = *(const uint8_t *)(raw + 37);
    green_shift = *(const uint8_t *)(raw + 38);
    blue_size = *(const uint8_t *)(raw + 39);
    blue_shift = *(const uint8_t *)(raw + 40);
    fb_enabled = (model == 1 && bpp == 32 && fb != 0);
    cursor_x = 0;
    cursor_y = 0;
}

void terminal_init(void) {
    fb_enabled = 0;
    cursor_x = 0;
    cursor_y = 0;
}

void terminal_clear(void) {
    if (fb_enabled) {
        uint32_t bg = rgb(8, 10, 14);
        for (uint32_t y = 0; y < height; ++y)
            for (uint32_t x = 0; x < width; ++x)
                fb_pixel(x, y, bg);
        cursor_x = 0;
        cursor_y = 0;
        return;
    }
    for (uint32_t y = 0; y < 25; ++y)
        for (uint32_t x = 0; x < 80; ++x)
            vga[y * 80 + x] = 0x0700 | ' ';
    cursor_x = 0;
    cursor_y = 0;
}

void terminal_putc(char c) {
    if (fb_enabled) {
        if (c == '\n') { fb_newline(); return; }
        if (c == '\b') {
            if (cursor_x) --cursor_x;
            fb_clear_glyph(cursor_x * 8, cursor_y * 10);
            return;
        }
        draw_char(c);
        if (++cursor_x * 8 >= width) fb_newline();
        return;
    }
    if (c == '\n') { cursor_x = 0; if (++cursor_y >= 25) cursor_y = 0; return; }
    if (c == '\b') { if (cursor_x) --cursor_x; vga[cursor_y * 80 + cursor_x] = 0x0700 | ' '; return; }
    vga[cursor_y * 80 + cursor_x] = 0x0700 | (unsigned char)c;
    if (++cursor_x >= 80) { cursor_x = 0; if (++cursor_y >= 25) cursor_y = 0; }
}

void terminal_write(const char *s) { while (*s) terminal_putc(*s++); }
void terminal_backspace(void) { terminal_putc('\b'); }
