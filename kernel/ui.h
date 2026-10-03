#ifndef UI_H
#define UI_H

#include <stddef.h>
#include <stdint.h>

void ui_clear_screen_color(uint8_t fg, uint8_t bg);
void ui_print_char(char c, uint8_t color, size_t x, size_t y);
void ui_print_string(const char* str, uint8_t color, size_t x, size_t y);
void ui_print_centered(const char* str, uint8_t color, size_t row);
void ui_draw_border(uint8_t color);
void ui_fill_row(size_t row, uint8_t color);
void ui_draw_box(size_t x, size_t y, size_t w, size_t h, uint8_t border_color, uint8_t fill_color);
void ui_scroll_area(size_t y1, size_t y2, uint8_t color);

#endif
