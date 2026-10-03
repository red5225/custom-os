#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>   // For uint8_t, uint16_t
#include <stddef.h>   // For size_t

#define VIDEO_ADDRESS 0xC00B8000
#define MAX_ROWS 25
#define MAX_COLS 80

// Color constants
typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GRAY = 7,
    VGA_COLOR_DARK_GRAY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW = 14,
    VGA_COLOR_WHITE = 15
} vga_color;

static inline uint8_t vga_entry_color(vga_color fg, vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}

void clear_screen(void);
void clear_screen_color(uint8_t fg, uint8_t bg);
void print_char(char c, uint8_t color, size_t x, size_t y);
void print_string(const char* str, uint8_t color, size_t x, size_t y);
void print_centered(const char* str, uint8_t color, size_t row);
void draw_border(uint8_t color);
void disable_cursor(void);
void fill_row(size_t row, uint8_t color);
void draw_box(size_t x, size_t y, size_t w, size_t h, uint8_t border_color, uint8_t fill_color);
// Scroll lines up within [y1, y2] inclusive; last line filled with color
void scroll_area(size_t y1, size_t y2, uint8_t color);

#endif