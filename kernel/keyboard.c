#include "keyboard.h"
#include "kernel.h"
#include "screen.h"
#include "ui.h"
#include "idt.h"
#include "log.h"

// Input line just above the status bar
static size_t kbd_row = MAX_ROWS - 3;
static size_t kbd_col = 2;
static const size_t prompt_x = 2;
static const char* prompt = "helixa> ";
static size_t prompt_len = 0; // computed at init

static uint8_t ui_color_text;
static uint8_t ui_color_bg;
static uint8_t ui_color_cursor; // inverted caret

// Input buffer
#define KBD_BUF_MAX  64
static char input_buf[KBD_BUF_MAX];
static size_t input_len = 0;
static size_t cursor = 0; // caret position within input

// History support
#define HIST_SIZE 10
static char hist[HIST_SIZE][KBD_BUF_MAX];
static size_t hist_count = 0; // number of stored entries
static size_t hist_head = 0;  // next slot to write
static int hist_pos = -1;     // -1 means editing new blank line

// Modifiers
static volatile uint8_t shift_down = 0;
static volatile uint8_t caps_lock = 0;
static volatile uint8_t ctrl_down = 0;
static volatile uint8_t e0_prefix = 0;

// Simple software key repeat using PIT (100Hz)
static volatile uint8_t repeat_active = 0;
static volatile uint8_t repeat_scancode = 0;
static volatile uint32_t repeat_delay = 0; // ticks countdown
static volatile uint32_t repeat_rate = 0;  // ticks countdown
#define REPEAT_START_TICKS 25  // ~250ms
#define REPEAT_RATE_TICKS   5  // ~50ms

// Caret blink (100 Hz base)
static volatile uint8_t caret_visible = 1;
static volatile uint32_t caret_ticks = 0;
#define CARET_BLINK_TICKS 50   // toggle every 0.5s -> 1Hz blink

// Log viewer toggle
static volatile uint8_t log_view = 0;

static const char scancode_map[128] = {
    0,   27, '1','2','3','4','5','6','7','8','9','0','-','=', 8,  9,
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',  0,  'a','s',
    'd','f','g','h','j','k','l',';','\'', '`', 0,  '\\','z','x','c','v',
    'b','n','m',',','.','/', 0,   '*', 0,  ' ', 0, 0, 0, 0, 0, 0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

static inline uint8_t color(uint8_t fg, uint8_t bg) { return vga_entry_color((vga_color)fg, (vga_color)bg); }

static char translate_char(uint8_t sc) {
    if (sc >= 128) return 0;
    char base = scancode_map[sc];
    if (!base) return 0;
    // Enter/backspace/tab/space pass through
    if (base == '\n' || base == 8 || base == 9 || base == ' ') return base;
    // Letters
    if (base >= 'a' && base <= 'z') {
        uint8_t upper = (shift_down ^ caps_lock);
        return upper ? (char)(base - 'a' + 'A') : base;
    }
    // Digits and punctuation with Shift
    if (shift_down) {
        switch (sc) {
            case 0x02: return '!'; // 1
            case 0x03: return '@'; // 2
            case 0x04: return '#'; // 3
            case 0x05: return '$'; // 4
            case 0x06: return '%'; // 5
            case 0x07: return '^'; // 6
            case 0x08: return '&'; // 7
            case 0x09: return '*'; // 8
            case 0x0A: return '('; // 9
            case 0x0B: return ')'; // 0
            case 0x0C: return '_'; // -
            case 0x0D: return '+'; // =
            case 0x1A: return '{'; // [
            case 0x1B: return '}'; // ]
            case 0x2B: return '|'; // backslash
            case 0x27: return ':'; // ;
            case 0x28: return '"'; // '
            case 0x29: return '~'; // `
            case 0x33: return '<'; // ,
            case 0x34: return '>';
            case 0x35: return '?'; // /
            case 0x39: return ' '; // space unchanged
        }
    }
    return base;
}

static void redraw_prompt_line(void) {
    // Clear entire input line
    ui_fill_row(kbd_row, ui_color_bg);
    // Draw prompt
    ui_print_string(prompt, ui_color_text, prompt_x, kbd_row);
    // Draw buffer
    for (size_t i = 0; i < input_len; i++) {
        if (prompt_x + prompt_len + i < MAX_COLS - 2)
            ui_print_char(input_buf[i], ui_color_text, prompt_x + prompt_len + i, kbd_row);
    }
    // Draw caret according to blink state
    size_t cx = prompt_x + prompt_len + (cursor <= input_len ? cursor : input_len);
    if (cx < MAX_COLS - 1) {
        char ch = (cursor < input_len) ? input_buf[cursor] : ' ';
        uint8_t col = caret_visible ? ui_color_cursor : ui_color_text;
    ui_print_char(ch, col, cx, kbd_row);
    }
    kbd_col = prompt_x + prompt_len + input_len;
}

static inline void caret_reset(void) { caret_visible = 1; caret_ticks = CARET_BLINK_TICKS; }

static void redraw_caret_only(void) {
    size_t cx = prompt_x + prompt_len + (cursor <= input_len ? cursor : input_len);
    if (cx < MAX_COLS - 1) {
        char ch = (cursor < input_len) ? input_buf[cursor] : ' ';
        uint8_t col = caret_visible ? ui_color_cursor : ui_color_text;
    ui_print_char(ch, col, cx, kbd_row);
    }
}

// (removed unused clear_output_row)

// Scrollable output region: rows [2 .. kbd_row-1]
static size_t out_row = 0; // initialized in init

static void print_output(const char* s) {
    if (out_row < 2 || out_row >= (kbd_row - 1)) out_row = kbd_row - 1;
    // advance to next line for each print
    if (out_row >= (kbd_row - 1)) {
        // scroll up the area and use last line (via UI abstraction)
        ui_scroll_area(2, kbd_row - 1, ui_color_bg);
        out_row = kbd_row - 1;
    }
    ui_fill_row(out_row, ui_color_bg);
    ui_print_string(s, ui_color_text, prompt_x, out_row);
    out_row++;
}

static void render_log_view(void) {
    // Clear output region and print most recent lines fitting on screen
    for (size_t r = 2; r < (kbd_row - 1); r++) ui_fill_row(r, ui_color_bg);
    size_t rows = (kbd_row - 1) - 2;
    size_t cnt = klog_line_count();
    size_t start = (cnt > rows) ? (cnt - rows) : 0;
    size_t y = 2;
    for (size_t i = start; i < cnt && y < (kbd_row - 1); i++, y++) {
        const char* ln = klog_get_line(i);
    if (ln) ui_print_string(ln, ui_color_text, prompt_x, y);
    }
    out_row = kbd_row - 1; // keep next output at bottom
}

static void handle_command(void) {
    // Simple commands
    if (input_len >= 4 && input_buf[0]=='e' && input_buf[1]=='c' && input_buf[2]=='h' && input_buf[3]=='o') {
        // Skip 'echo' and optional space
        size_t i = 4; if (i < input_len && input_buf[i] == ' ') i++;
        // Copy to temp buffer capped to row width
        char out[80]; size_t j = 0;
        while (i < input_len && j < sizeof(out)-1) out[j++] = input_buf[i++];
        out[j] = '\0';
        print_output(out);
    } else if (input_len == 4 && input_buf[0]=='h' && input_buf[1]=='e' && input_buf[2]=='l' && input_buf[3]=='p') {
        print_output("Commands: help, clear, time, echo <msg>, reboot");
    } else if (input_len == 5 && input_buf[0]=='c' && input_buf[1]=='l' && input_buf[2]=='e' && input_buf[3]=='a' && input_buf[4]=='r') {
        // Clear output region
    for (size_t r = 2; r < (kbd_row - 1); r++) ui_fill_row(r, ui_color_bg);
        out_row = 2;
    } else if (input_len == 4 && input_buf[0]=='t' && input_buf[1]=='i' && input_buf[2]=='m' && input_buf[3]=='e') {
        // Show uptime HH:MM:SS
        uint32_t t = timer_ticks() / 100; // 100 Hz
        uint32_t s = t % 60;
        uint32_t m = (t / 60) % 60;
        uint32_t h = (t / 3600) % 24;
        char buf[32];
        buf[0]='0'+(h/10); buf[1]='0'+(h%10); buf[2]=':'; buf[3]='0'+(m/10); buf[4]='0'+(m%10);
        buf[5]=':'; buf[6]='0'+(s/10); buf[7]='0'+(s%10); buf[8]='\0';
        print_output(buf);
    } else if (input_len == 6 && input_buf[0]=='r' && input_buf[1]=='e' && input_buf[2]=='b' && input_buf[3]=='o' && input_buf[4]=='o' && input_buf[5]=='t') {
        print_output("Rebooting...");
        // Keyboard controller reboot
        // Wait for input buffer empty
        while (port_byte_in(0x64) & 0x02) {}
        port_byte_out(0x64, 0xFE);
        for(;;) { __asm__ __volatile__("hlt"); }
    } else if (input_len == 4 && input_buf[0]=='o' && input_buf[1]=='o' && input_buf[2]=='p' && input_buf[3]=='s') {
        print_output("Triggering INT3...");
        __asm__ __volatile__("int $3");
    } else if (input_len == 4 && input_buf[0]=='d' && input_buf[1]=='i' && input_buf[2]=='v' && input_buf[3]=='0') {
        print_output("Triggering divide-by-zero...");
        volatile uint32_t a = 1, b = 0, c;
        __asm__ __volatile__("div %%ecx" : "=a"(c) : "a"(a), "c"(b) : "edx");
        (void)c;
    } else if (input_len == 2 && input_buf[0]=='p' && input_buf[1]=='f') {
        print_output("Triggering page fault...");
        volatile uint32_t *p = (uint32_t*)0xDEADBEEF;
        *p = 0x12345678;
    } else if (input_len == 0) {
        // nothing
    } else {
        print_output("Unknown command. Try: echo hello");
    }
    // Save to history if non-empty
    if (input_len > 0) {
        size_t n = (input_len < KBD_BUF_MAX-1) ? input_len : (KBD_BUF_MAX-1);
        for (size_t i = 0; i < n; i++) hist[hist_head][i] = input_buf[i];
        hist[hist_head][n] = '\0';
        hist_head = (hist_head + 1) % HIST_SIZE;
        if (hist_count < HIST_SIZE) hist_count++;
    }
    // Reset buffer and redraw prompt
    input_len = 0; cursor = 0; hist_pos = -1;
    redraw_prompt_line();
}

void keyboard_init(void) {
    // prepare input line UI
    ui_color_text = color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    ui_color_bg   = color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    ui_color_cursor = color(VGA_COLOR_BLUE, VGA_COLOR_WHITE);
    ui_fill_row(kbd_row, ui_color_bg);
    // prompt
    ui_print_string(prompt, ui_color_text, prompt_x, kbd_row);
    prompt_len = 0; while (prompt[prompt_len]) prompt_len++;
    kbd_col = prompt_x + prompt_len;
    input_len = 0; cursor = 0;
    out_row = 2;

    // Unmask IRQ1 on PIC1
    uint8_t mask = port_byte_in(0x21);
    mask &= ~(1 << 1);
    port_byte_out(0x21, mask);

    // Reset modifiers, repeat, history, caret
    shift_down = 0; caps_lock = 0; ctrl_down = 0; e0_prefix = 0;
    repeat_active = 0; repeat_scancode = 0; repeat_delay = 0; repeat_rate = 0;
    hist_count = 0; hist_head = 0; hist_pos = -1;
    caret_visible = 1; caret_ticks = CARET_BLINK_TICKS;
}

void keyboard_isr_handler(void) {
    uint8_t sc = port_byte_in(0x60);
    if (sc == 0xE0) { e0_prefix = 1; goto eoi; }
    uint8_t release = sc & 0x80; uint8_t code = sc & 0x7F;
    if (release) {
        // handle modifier releases
        if (code == 0x2A || code == 0x36) shift_down = 0;
        if (code == 0x1D) ctrl_down = 0;
        // if releasing the repeating key, stop repeat
        if (repeat_active && code == repeat_scancode) { repeat_active = 0; }
        e0_prefix = 0;
    } else {
        // modifiers
        if (code == 0x2A || code == 0x36) { shift_down = 1; goto eoi; }
        if (code == 0x1D) { ctrl_down = 1; goto eoi; }
        if (code == 0x3A) { caps_lock = !caps_lock; goto eoi; }

        // E0 navigation keys
        if (e0_prefix) {
            if (code == 0x4B) { // Left
                if (cursor > 0) cursor--;
                caret_reset(); redraw_prompt_line();
            } else if (code == 0x4D) { // Right
                if (cursor < input_len) cursor++;
                caret_reset(); redraw_prompt_line();
            } else if (code == 0x47) { // Home
                cursor = 0; caret_reset(); redraw_prompt_line();
            } else if (code == 0x4F) { // End
                cursor = input_len; caret_reset(); redraw_prompt_line();
            } else if (code == 0x53) { // Delete
                if (cursor < input_len) {
                    for (size_t i = cursor; i + 1 <= input_len; i++) input_buf[i] = input_buf[i+1];
                    input_len--; caret_reset(); redraw_prompt_line();
                }
            } else if (code == 0x48) { // Up (history older)
                if (hist_count > 0) {
                    if (hist_pos == -1) hist_pos = (int)((hist_head + HIST_SIZE - 1) % HIST_SIZE);
                    else if (hist_pos != (int)((hist_head + HIST_SIZE - hist_count) % HIST_SIZE))
                        hist_pos = (hist_pos + HIST_SIZE - 1) % HIST_SIZE;
                    size_t i = 0; while (i < KBD_BUF_MAX-1 && hist[hist_pos][i]) { input_buf[i] = hist[hist_pos][i]; i++; }
                    input_len = i; input_buf[input_len] = '\0'; cursor = input_len; caret_reset(); redraw_prompt_line();
                }
            } else if (code == 0x50) { // Down (history newer)
                if (hist_count > 0 && hist_pos != -1) {
                    int newest = (int)((hist_head + HIST_SIZE - 1) % HIST_SIZE);
                    if (hist_pos == newest) { hist_pos = -1; input_len = 0; cursor = 0; }
                    else { hist_pos = (hist_pos + 1) % HIST_SIZE; size_t i = 0; while (i < KBD_BUF_MAX-1 && hist[hist_pos][i]) { input_buf[i] = hist[hist_pos][i]; i++; } input_len = i; input_buf[input_len] = '\0'; cursor = input_len; }
                    caret_reset(); redraw_prompt_line();
                }
            }
            e0_prefix = 0; goto eoi;
        }

        char c = translate_char(code);
        // Ctrl shortcuts
        if (ctrl_down) {
            if (c == 'u' || c == 'U') { input_len = 0; cursor = 0; caret_reset(); redraw_prompt_line(); repeat_active = 0; goto eoi; }
            if (c == 'w' || c == 'W') {
                while (cursor > 0 && input_buf[cursor-1] == ' ') { for (size_t i = cursor-1; i < input_len; i++) input_buf[i] = input_buf[i+1]; cursor--; input_len--; }
                while (cursor > 0 && cursor <= input_len && input_buf[cursor-1] != ' ') { for (size_t i = cursor-1; i < input_len; i++) input_buf[i] = input_buf[i+1]; cursor--; input_len--; }
                caret_reset(); redraw_prompt_line(); repeat_active = 0; goto eoi;
            }
            if (c == 'c' || c == 'C') { input_len = 0; cursor = 0; caret_reset(); redraw_prompt_line(); repeat_active = 0; goto eoi; }
            if (c == 'l' || c == 'L') { log_view = !log_view; if (log_view) render_log_view(); else { for (size_t r=2;r<(kbd_row-1);r++) fill_row(r, ui_color_bg); out_row = 2; } goto eoi; }
        }
        if (c == '\n') {
            handle_command();
            repeat_active = 0;
        } else if (c == 8) {
            if (cursor > 0) { for (size_t i = cursor-1; i < input_len; i++) input_buf[i] = input_buf[i+1]; cursor--; input_len--; caret_reset(); redraw_prompt_line(); }
            repeat_active = 0; // don't repeat backspace for now
        } else if (c) {
            if (input_len < KBD_BUF_MAX - 1 && (prompt_x + prompt_len + input_len) < MAX_COLS - 2) {
                for (size_t i = input_len; i > cursor; i--) input_buf[i] = input_buf[i-1];
                input_buf[cursor] = c; input_len++; cursor++; caret_reset(); redraw_prompt_line();
                repeat_active = 1; repeat_scancode = code; repeat_delay = REPEAT_START_TICKS; repeat_rate = REPEAT_RATE_TICKS;
            }
        }
    }
eoi:
    // EOI
    port_byte_out(0x20, 0x20);
}

void keyboard_on_timer_tick(void) {
    // Key repeat
    if (repeat_active) {
        if (repeat_delay) { repeat_delay--; }
        else if (repeat_rate) { repeat_rate--; }
        else {
            repeat_rate = REPEAT_RATE_TICKS;
            char c = translate_char(repeat_scancode);
            if (c && c != '\n' && c != 8) {
                if (input_len < KBD_BUF_MAX - 1 && (prompt_x + prompt_len + input_len) < MAX_COLS - 2) {
                    for (size_t i = input_len; i > cursor; i--) input_buf[i] = input_buf[i-1];
                    input_buf[cursor] = c; input_len++; cursor++;
                    caret_reset(); redraw_prompt_line();
                }
            }
        }
    }
    // Caret blink
    if (caret_ticks) caret_ticks--; else { caret_ticks = CARET_BLINK_TICKS; caret_visible = !caret_visible; if (!log_view) redraw_caret_only(); }
}