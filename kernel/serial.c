#include "serial.h"
#include "kernel.h"
#include "log.h"

#define COM1 0x3F8

void serial_init(void) {
    port_byte_out(COM1 + 1, 0x00);    // Disable all interrupts
    port_byte_out(COM1 + 3, 0x80);    // Enable DLAB
    port_byte_out(COM1 + 0, 0x03);    // 38400 baud (divisor 3)
    port_byte_out(COM1 + 1, 0x00);
    port_byte_out(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    port_byte_out(COM1 + 2, 0xC7);    // Enable FIFO, clear, 14-byte threshold
    port_byte_out(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

static int serial_is_transmit_empty() {
    return port_byte_in(COM1 + 5) & 0x20;
}

void serial_write_char(char c) {
    // Always mirror into kernel log
    klog_write_char(c);
    #ifdef RELEASE
    // Quiet serial in release
    (void)c; return;
    #endif
    while (!serial_is_transmit_empty());
    port_byte_out(COM1, (uint8_t)c);
}

void serial_write(const char* s) {
    for (int i = 0; s[i]; i++) serial_write_char(s[i]);
}

void serial_write_hex(uint32_t v) {
    const char* hex = "0123456789ABCDEF";
    serial_write("0x");
    for (int i = 7; i >= 0; i--) {
        serial_write_char(hex[(v >> (i*4)) & 0xF]);
    }
}

void serial_write_dec(uint32_t v) {
    char buf[11]; int i = 0;
    if (v == 0) { serial_write("0"); return; }
    while (v && i < 10) { buf[i++] = '0' + (v % 10); v /= 10; }
    while (i--) serial_write_char(buf[i]);
}