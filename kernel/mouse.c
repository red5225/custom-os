#include "mouse.h"
#include "kernel.h"
#include <stdint.h>

#define KBD_STATUS 0x64
#define KBD_DATA   0x60
#define MOUSE_CMD  0x64

static int mouse_ready = 0;
static int mouse_cycle = 0;
static uint8_t mouse_packet[3];
static int mouse_x = 0;
static int mouse_y = 0;
static uint8_t mouse_buttons = 0;

static void wait_write(void) {
    for (volatile int i = 0; i < 100000; ++i)
        if (!(port_byte_in(KBD_STATUS) & 2)) return;
}
static void wait_read(void) {
    for (volatile int i = 0; i < 100000; ++i)
        if (port_byte_in(KBD_STATUS) & 1) return;
}

static void mouse_write(uint8_t value) {
    wait_write();
    port_byte_out(MOUSE_CMD, 0xD4);
    wait_write();
    port_byte_out(KBD_DATA, value);
    wait_read();
    (void)port_byte_in(KBD_DATA);
}

void mouse_init(void) {
    /* Enable the auxiliary PS/2 mouse port. */
    wait_write();
    port_byte_out(MOUSE_CMD, 0xA8);

    /* Enable mouse data reporting. */
    mouse_write(0xF4);

    mouse_cycle = 0;
    mouse_ready = 1;
    mouse_x = 0;
    mouse_y = 0;
    mouse_buttons = 0;
}

void mouse_poll(void) {
    if (!mouse_ready) return;

    for (int n = 0; n < 8; ++n) {
        uint8_t status = port_byte_in(KBD_STATUS);
        if (!(status & 1)) return;

        /* Status bit 5 means the byte came from the mouse. */
        if (!(status & 0x20)) {
            (void)port_byte_in(KBD_DATA);
            continue;
        }

        uint8_t data = port_byte_in(KBD_DATA);

        if (mouse_cycle == 0) {
            /* Bit 3 must be set in the first PS/2 mouse byte. */
            if (!(data & 0x08)) continue;
        }

        mouse_packet[mouse_cycle++] = data;

        if (mouse_cycle == 3) {
            int dx = (int)(int8_t)mouse_packet[1];
            int dy = (int)(int8_t)mouse_packet[2];

            if (!(mouse_packet[0] & 0x40)) mouse_x += dx;
            if (!(mouse_packet[0] & 0x80)) mouse_y -= dy;

            if (mouse_x < 0) mouse_x = 0;
            if (mouse_y < 0) mouse_y = 0;
            if (mouse_x > 639) mouse_x = 639;
            if (mouse_y > 479) mouse_y = 479;

            mouse_buttons = mouse_packet[0] & 0x07;
            mouse_cycle = 0;
        }
    }
}

void mouse_isr_handler(void) {
    mouse_poll();
}

void mouse_get(int *x, int *y, unsigned char *buttons) {
    if (x) *x = mouse_x;
    if (y) *y = mouse_y;
    if (buttons) *buttons = mouse_buttons;
}
