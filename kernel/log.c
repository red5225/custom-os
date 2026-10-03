#include "log.h"
#include <stddef.h>

#define KLOG_LINES 64
#define KLOG_COLS  80

static char lines[KLOG_LINES][KLOG_COLS];
static size_t line_len[KLOG_LINES];
static size_t head = 0;     // next write line index
static size_t count = 0;    // number of valid lines

static void new_line(void) {
    head = (head + 1) % KLOG_LINES;
    line_len[head] = 0;
    if (count < KLOG_LINES) count++; // grows until full
}

void klog_init(void) {
    for (size_t i = 0; i < KLOG_LINES; i++) { line_len[i] = 0; lines[i][0] = '\0'; }
    head = 0; count = 0;
}

void klog_write_char(char c) {
    if (c == '\r') return; // ignore CR
    if (c == '\n') { new_line(); return; }
    size_t len = line_len[head];
    if (len >= KLOG_COLS - 1) { new_line(); len = 0; }
    lines[head][len++] = c; lines[head][len] = '\0';
    line_len[head] = len;
}

void klog_write(const char* s) { while (*s) klog_write_char(*s++); }

void klog_write_hex(uint32_t v) {
    static const char* hex = "0123456789ABCDEF";
    klog_write("0x");
    for (int i = 7; i >= 0; i--) klog_write_char(hex[(v >> (i*4)) & 0xF]);
}

void klog_write_dec(uint32_t v) {
    char buf[11]; int i = 0;
    if (v == 0) { klog_write("0"); return; }
    while (v && i < 10) { buf[i++] = '0' + (v % 10); v /= 10; }
    while (i--) klog_write_char(buf[i]);
}

size_t klog_line_count(void) { return count; }

const char* klog_get_line(size_t index) {
    if (index >= count) return NULL;
    size_t oldest = (count == KLOG_LINES) ? ((head + 1) % KLOG_LINES) : 0;
    size_t idx = (oldest + index) % KLOG_LINES;
    return lines[idx];
}
