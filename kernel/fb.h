#ifndef FB_H
#define FB_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t addr;    // physical address
    uint32_t pitch;   // bytes per line
    uint32_t width;
    uint32_t height;
    uint8_t  bpp;     // bits per pixel
    uint8_t  type;    // 1 RGB, 2 indexed, etc.
    // RGB masks (for type=1)
    uint8_t red_pos, red_mask;
    uint8_t green_pos, green_mask;
    uint8_t blue_pos, blue_mask;
} fb_info_t;

int fb_init_from_multiboot(uint32_t mb2_info);
int fb_is_available(void);
void fb_fill(uint32_t argb);
void fb_putpixel(uint32_t x, uint32_t y, uint32_t argb);
void fb_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t argb);
void fb_blit_glyph(uint32_t x, uint32_t y, char ch, uint32_t fg, uint32_t bg);
void fb_blit_glyph_scaled(uint32_t x, uint32_t y, char ch, uint32_t fg, uint32_t bg, uint32_t scale);
void fb_draw_string(uint32_t x, uint32_t y, const char* s, uint32_t fg, uint32_t bg);
void fb_draw_string_scaled(uint32_t x, uint32_t y, const char* s, uint32_t fg, uint32_t bg, uint32_t scale);
uint32_t fb_width(void);
uint32_t fb_height(void);

// Optional helpers used by GUI for cursor save/restore (only in 32bpp linear RGB)
int fb_copy_out32(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t* dst);
int fb_copy_in32(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t* src);

// Blit an ARGB32 buffer to the framebuffer (converts to native format)
int fb_blit_argb32(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t* src, uint32_t srcStridePx);
// Copy a rectangle within the framebuffer (like memmove for pixel rects). Handles overlap. Returns 1 on success.
int fb_copy_rect(uint32_t srcx, uint32_t srcy, uint32_t w, uint32_t h, uint32_t dstx, uint32_t dsty);

#endif
