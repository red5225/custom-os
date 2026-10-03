#include "ui.h"
#include "screen.h" // for MAX_ROWS/COLS and color enums
#include "fb.h"

// Map 4-bit VGA palette index to ARGB true color (basic approximation)
static uint32_t vga_to_argb(uint8_t idx) {
    switch (idx & 0x0F) {
        case 0:  return 0xFF000000; // black
        case 1:  return 0xFF0000AA; // blue
        case 2:  return 0xFF00AA00; // green
        case 3:  return 0xFF00AAAA; // cyan
        case 4:  return 0xFFAA0000; // red
        case 5:  return 0xFFAA00AA; // magenta
        case 6:  return 0xFFAA5500; // brown
        case 7:  return 0xFFAAAAAA; // light gray
        case 8:  return 0xFF555555; // dark gray
        case 9:  return 0xFF5555FF; // light blue
        case 10: return 0xFF55FF55; // light green
        case 11: return 0xFF55FFFF; // light cyan
        case 12: return 0xFFFF5555; // light red
        case 13: return 0xFFFF55FF; // light magenta
        case 14: return 0xFFFFFF55; // yellow
        case 15: return 0xFFFFFFFF; // white
    }
    return 0xFFFFFFFF;
}

static void glyph_draw(uint32_t x, uint32_t y, char ch, uint8_t color) {
    uint8_t fg_idx = (color & 0x0F);
    uint8_t bg_idx = (color >> 4) & 0x0F;
    uint32_t fg = vga_to_argb(fg_idx);
    uint32_t bg = vga_to_argb(bg_idx);
    fb_blit_glyph(x, y, ch, fg, bg);
}

void ui_clear_screen_color(uint8_t fg, uint8_t bg) {
    (void)fg; (void)bg; // not needed per glyph
    uint32_t bgc = vga_to_argb(bg & 0x0F);
    fb_fill(bgc);
}

void ui_print_char(char c, uint8_t color, size_t x, size_t y) {
    glyph_draw((uint32_t)(x*8), (uint32_t)(y*8), c, color);
}

void ui_print_string(const char* str, uint8_t color, size_t x, size_t y) {
    uint32_t px = (uint32_t)(x*8), py = (uint32_t)(y*8);
    uint32_t fg = vga_to_argb(color & 0x0F), bg = vga_to_argb((color>>4)&0x0F);
    for (size_t i=0; str[i]; i++, px+=8) fb_blit_glyph(px, py, str[i], fg, bg);
}

void ui_print_centered(const char* str, uint8_t color, size_t row) {
    size_t len = 0; while (str[len]) len++;
    size_t col = (len < MAX_COLS) ? (MAX_COLS - len) / 2 : 0;
    ui_print_string(str, color, col, row);
}

void ui_draw_border(uint8_t color) {
    const char H = (char)0xCD, V = (char)0xBA, TL=(char)0xC9, TR=(char)0xBB, BL=(char)0xC8, BR=(char)0xBC;
    for (size_t x=0;x<MAX_COLS;x++){ ui_print_char(H,color,x,0); ui_print_char(H,color,x,MAX_ROWS-1);}    
    for (size_t y=0;y<MAX_ROWS;y++){ ui_print_char(V,color,0,y); ui_print_char(V,color,MAX_COLS-1,y);}    
    ui_print_char(TL,color,0,0); ui_print_char(TR,color,MAX_COLS-1,0); ui_print_char(BL,color,0,MAX_ROWS-1); ui_print_char(BR,color,MAX_COLS-1,MAX_ROWS-1);
}

void ui_fill_row(size_t row, uint8_t color) {
    for (size_t x=0;x<MAX_COLS;x++) ui_print_char(' ', color, x, row);
}

void ui_draw_box(size_t x, size_t y, size_t w, size_t h, uint8_t border_color, uint8_t fill_color) {
    if (w == 0 || h == 0) return; 
    size_t x2 = x + w - 1, y2 = y + h - 1; 
    if (x2 >= MAX_COLS) x2 = MAX_COLS - 1; 
    if (y2 >= MAX_ROWS) y2 = MAX_ROWS - 1;
    for (size_t yy = y + 1; yy < y2; yy++) 
        for (size_t xx = x + 1; xx < x2; xx++) 
            ui_print_char(' ', fill_color, xx, yy);
    const char H = (char)0xC4, V = (char)0xB3, TL = (char)0xDA, TR = (char)0xBF, BL = (char)0xC0, BR = (char)0xD9;
    for (size_t xx = x; xx <= x2; xx++) { 
        ui_print_char(H, border_color, xx, y); 
        ui_print_char(H, border_color, xx, y2);
    }    
    for (size_t yy = y; yy <= y2; yy++) { 
        ui_print_char(V, border_color, x, yy); 
        ui_print_char(V, border_color, x2, yy);
    }   
    ui_print_char(TL, border_color, x, y); 
    ui_print_char(TR, border_color, x2, y); 
    ui_print_char(BL, border_color, x, y2); 
    ui_print_char(BR, border_color, x2, y2);
}

void ui_scroll_area(size_t y1, size_t y2, uint8_t color) {
    if (y1 >= y2 || y2 >= MAX_ROWS) return;
    // Smooth scroll: copy pixel rect up by one character row (8px) and clear last row
    uint32_t top_px = (uint32_t)(y1 * 8);
    uint32_t bottom_px = (uint32_t)(y2 * 8);
    uint32_t height_px = bottom_px - top_px;
    if (height_px >= 8) {
        fb_copy_rect(0, top_px + 8, (uint32_t)(MAX_COLS * 8), height_px - 8, 0, top_px);
    }
    // Clear last text row in the area
    uint32_t bg = vga_to_argb((color >> 4) & 0x0F);
    fb_rect(0, bottom_px - 8, (uint32_t)(MAX_COLS * 8), 8, bg);
}
