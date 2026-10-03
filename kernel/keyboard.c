#include "keyboard.h"
#include "kernel.h"
#include "screen.h"
#include <stdint.h>

#define STATUS 0x64
#define DATA 0x60

static char input[64];
static int len;
static uint8_t shift;

static const unsigned char map[59] = {
0,27,49,50,51,52,53,54,55,56,57,48,45,61,9,
113,119,101,114,116,121,117,105,111,112,91,93,10,0,
97,115,100,102,103,104,106,107,108,59,39,96,0,92,
122,120,99,118,98,110,109,44,46,47,0,42,32,0
};

static char shifted_char(uint8_t s) {
    unsigned char c = map[s];
    if (c >= 97 && c <= 122) c = (unsigned char)(c - 32);
    switch (s) {
        case 2: return 33; case 3: return 64; case 4: return 35;
        case 5: return 36; case 6: return 37; case 7: return 94;
        case 8: return 38; case 9: return 42; case 10: return 40;
        case 11: return 41; case 12: return 95; case 13: return 43;
        default: return (char)c;
    }
}

static void redraw(void) {
    print_string(input, vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), 4, 10);
}

static void run_cmd(void) {
    input[len] = 0;
    print_string("\n", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), 2, 12);

    if (len == 4 && input[0]==104 && input[1]==101 && input[2]==108 && input[3]==112)
        print_string("help  clear  echo  reboot", vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK), 4, 13);
    else if (len == 5 && input[0]==99 && input[1]==108 && input[2]==101 && input[3]==97 && input[4]==114)
        clear_screen_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK);
    else if (len >= 5 && input[0]==101 && input[1]==99 && input[2]==104 && input[3]==111 && input[4]==32)
        print_string(input + 5, vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK), 4, 13);
    else
        print_string("Unknown command", vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK), 4, 13);

    len = 0;
    input[0] = 0;
    print_string("> ", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), 2, 15);
}

void keyboard_init(void) {
    len = 0;
    shift = 0;
    input[0] = 0;
}

void keyboard_poll(void) {
    for (int n=0; n<8; n++) {
        uint8_t status = port_byte_in(STATUS);
        if (!(status & 1)) return;
        uint8_t sc = port_byte_in(DATA);
        if (status & 0x20) continue;

        if (sc == 0x2A || sc == 0x36) { shift=1; continue; }
        if (sc == 0xAA || sc == 0xB6) { shift=0; continue; }
        if (sc & 0x80) continue;
        if (sc == 0x1C) { run_cmd(); continue; }
        if (sc == 0x0E) {
            if (len) { --len; input[len]=0; redraw(); }
            continue;
        }
        if (sc < 59) {
            char c = shift ? shifted_char(sc) : (char)map[sc];
            if (c && len < 63) { input[len++]=c; input[len]=0; redraw(); }
        }
    }
}

void keyboard_isr_handler(void) { keyboard_poll(); }
void keyboard_on_timer_tick(void) {}
