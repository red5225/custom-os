#include "screen.h"
#include "kernel.h"
#include <stddef.h>
#include <stdint.h>

static uint16_t* video_memory = (uint16_t*)VIDEO_ADDRESS;

void clear_screen() {
    for (size_t y = 0; y < MAX_ROWS; y++) {
        for (size_t x = 0; x < MAX_COLS; x++) {
            const size_t index = y * MAX_COLS + x;
            video_memory[index] = vga_entry(' ', 
                vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
        }
    }
}

void clear_screen_color(uint8_t fg, uint8_t bg) {
    const uint8_t color = vga_entry_color(fg, bg);
    for (size_t y = 0; y < MAX_ROWS; y++) {
        for (size_t x = 0; x < MAX_COLS; x++) {
            const size_t index = y * MAX_COLS + x;
            video_memory[index] = vga_entry(' ', color);
        }
    }
}

void print_char(char c, uint8_t color, size_t x, size_t y) {
    if (x >= MAX_COLS || y >= MAX_ROWS) return;
    const size_t index = y * MAX_COLS + x;
    video_memory[index] = vga_entry(c, color);
}

void print_string(const char* str, uint8_t color, size_t x, size_t y) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        print_char(str[i], color, x + i, y);
    }
}

void print_centered(const char* str, uint8_t color, size_t row) {
    size_t len = 0; while (str[len]) len++;
    size_t col = 0;
    if (len < MAX_COLS) col = (MAX_COLS - len) / 2;
    print_string(str, color, col, row);
}

void draw_border(uint8_t color) {
    // CP437 double-line border: âââ â âââ
    const char H = (char)0xCD;  // 'â'
    const char V = (char)0xBA;  // 'â'
    const char TL = (char)0xC9; // 'â'
    const char TR = (char)0xBB; // 'â'
    const char BL = (char)0xC8; // 'â'
    const char BR = (char)0xBC; // 'â'

    for (size_t x = 0; x < MAX_COLS; x++) {
        print_char(H, color, x, 0);
        print_char(H, color, x, MAX_ROWS - 1);
    }
    for (size_t y = 0; y < MAX_ROWS; y++) {
        print_char(V, color, 0, y);
        print_char(V, color, MAX_COLS - 1, y);
    }
    print_char(TL, color, 0, 0);
    print_char(TR, color, MAX_COLS - 1, 0);
    print_char(BL, color, 0, MAX_ROWS - 1);
    print_char(BR, color, MAX_COLS - 1, MAX_ROWS - 1);
}

void disable_cursor(void) {
    // VGA cursor disable via ports 0x3D4/0x3D5
    // Reference: set bit 5 of Cursor Start register (index 0x0A)
    port_byte_out(0x3D4, 0x0A);
    uint8_t cur_start = port_byte_in(0x3D5);
    cur_start |= 0x20; // disable cursor
    port_byte_out(0x3D5, cur_start);
}

void fill_row(size_t row, uint8_t color) {
    if (row >= MAX_ROWS) return;
    for (size_t x = 0; x < MAX_COLS; x++) {
        print_char(' ', color, x, row);
    }
}

void draw_box(size_t x, size_t y, size_t w, size_t h, uint8_t border_color, uint8_t fill_color) {
    if (x >= MAX_COLS || y >= MAX_ROWS) return;
    if (w == 0 || h == 0) return;
    size_t x2 = x + w - 1;
    size_t y2 = y + h - 1;
    if (x2 >= MAX_COLS) x2 = MAX_COLS - 1;
    if (y2 >= MAX_ROWS) y2 = MAX_ROWS - 1;

    // Fill interior
    for (size_t yy = y + 1; yy < y2; yy++) {
        for (size_t xx = x + 1; xx < x2; xx++) {
            print_char(' ', fill_color, xx, yy);
        }
    }

    // Single-line CP437 inner border: âââ â âââ
    const char H = (char)0xC4;  // 'â'
    const char V = (char)0xB3;  // 'â'
    const char TL = (char)0xDA; // 'â'
    const char TR = (char)0xBF; // 'â'
    const char BL = (char)0xC0; // 'â'
    const char BR = (char)0xD9; // 'â'

    for (size_t xx = x; xx <= x2; xx++) {
        print_char(H, border_color, xx, y);
        print_char(H, border_color, xx, y2);
    }
    for (size_t yy = y; yy <= y2; yy++) {
        print_char(V, border_color, x, yy);
        print_char(V, border_color, x2, yy);
    }
    print_char(TL, border_color, x, y);
    print_char(TR, border_color, x2, y);
    print_char(BL, border_color, x, y2);
    print_char(BR, border_color, x2, y2);
}

void scroll_area(size_t y1, size_t y2, uint8_t color) {
    if (y1 >= y2 || y2 >= MAX_ROWS) return;
    // Move each row up by one within the area (memmove-style)
    for (size_t y = y1; y < y2; y++) {
        for (size_t x = 0; x < MAX_COLS; x++) {
            video_memory[y * MAX_COLS + x] = video_memory[(y + 1) * MAX_COLS + x];
        }
    }
    // Clear last line
    uint16_t blank = vga_entry(' ', color);
    for (size_t x = 0; x < MAX_COLS; x++) video_memory[y2 * MAX_COLS + x] = blank;
}