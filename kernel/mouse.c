#include "mouse.h"
#include "kernel.h"
#include "serial.h"

static volatile int mx = 100, my = 100; // start somewhere visible
static volatile uint8_t mbtn = 0;
static volatile uint8_t packet[3];
static volatile int pidx = 0;

// Mouse acceleration for smooth movement
static int apply_mouse_acceleration(int delta) {
    if (delta == 0) return 0;
    
    int abs_delta = (delta < 0) ? -delta : delta;
    int sign = (delta < 0) ? -1 : 1;
    
    // Apply acceleration curve like Windows/macOS
    int result;
    if (abs_delta <= 2) {
        result = abs_delta; // Slow movement: 1:1
    } else if (abs_delta <= 6) {
        result = abs_delta + 1; // Medium movement: slight acceleration
    } else {
        result = abs_delta * 2; // Fast movement: 2x acceleration
    }
    
    return result * sign;
}

static int wait_write_t(void){
    for (uint32_t i=0; i<100000; ++i) { if (!(port_byte_in(0x64) & 0x02)) return 1; }
    return 0;
}
static int wait_read_t(void){
    for (uint32_t i=0; i<100000; ++i) { if (port_byte_in(0x64) & 0x01) return 1; }
    return 0;
}
static int mouse_write(uint8_t val){ if (!wait_write_t()) return 0; port_byte_out(0x64, 0xD4); if (!wait_write_t()) return 0; port_byte_out(0x60, val); return 1; }
static int controller_cmd(uint8_t cmd, uint8_t* out){ if (!wait_write_t()) return 0; port_byte_out(0x64, cmd); if (!wait_read_t()) return 0; if (out) *out = port_byte_in(0x60); else (void)port_byte_in(0x60); return 1; }

void mouse_init(void) {
    serial_write("[mouse] starting initialization\r\n");
    
    // Unmask IRQ2 (cascade) and IRQ12 on PICs
    uint8_t m = port_byte_in(0x21); m &= ~0x04; port_byte_out(0x21, m);
    uint8_t s = port_byte_in(0xA1); s &= ~0x10; port_byte_out(0xA1, s);
    serial_write("[mouse] PICs unmasked\r\n");

    // Enable auxiliary device (may fail under some emulations)
    if (!wait_write_t()) { serial_write("[mouse] timeout wait_write A8\r\n"); return; }
    port_byte_out(0x64, 0xA8);
    serial_write("[mouse] sent enable aux device command\r\n");
    
    // Read command byte, set IRQ12 enable, write back
    uint8_t status = 0;
    if (!controller_cmd(0x20, &status)) { 
        serial_write("[mouse] no cmd byte\r\n"); 
    } else {
        serial_write("[mouse] read command byte successfully\r\n");
    }
    status |= 0x02; // enable IRQ12
    status &= ~0x20; // disable mouse clock
    if (!wait_write_t()) { serial_write("[mouse] timeout before 0x60\r\n"); return; }
    port_byte_out(0x64, 0x60);
    if (!wait_write_t()) { serial_write("[mouse] timeout data write cmd byte\r\n"); return; }
    port_byte_out(0x60, status);
    serial_write("[mouse] wrote command byte back\r\n");

    // Reset mouse to defaults
    if (!mouse_write(0xFF) || !wait_read_t()) { 
        serial_write("[mouse] reset failed\r\n"); 
    } else { 
        (void)port_byte_in(0x60); // consume ACK
        // Wait for completion bytes
        if (wait_read_t()) (void)port_byte_in(0x60); // 0xAA
        if (wait_read_t()) (void)port_byte_in(0x60); // 0x00 
    }

    // Set sampling rate to 100 reports/sec (smooth movement)
    if (!mouse_write(0xF3) || !wait_read_t()) { 
        serial_write("[mouse] sample rate cmd failed\r\n"); 
    } else { 
        (void)port_byte_in(0x60); // ACK
        if (!mouse_write(100) || !wait_read_t()) {
            serial_write("[mouse] sample rate data failed\r\n");
        } else {
            (void)port_byte_in(0x60); // ACK
        }
    }

    // Set resolution to 200 dpi (good balance)
    if (!mouse_write(0xE8) || !wait_read_t()) { 
        serial_write("[mouse] resolution cmd failed\r\n"); 
    } else { 
        (void)port_byte_in(0x60); // ACK
        if (!mouse_write(3) || !wait_read_t()) { // 3 = 200 dpi
            serial_write("[mouse] resolution data failed\r\n");
        } else {
            (void)port_byte_in(0x60); // ACK
        }
    }

    // Enable data reporting
    if (!mouse_write(0xF4) || !wait_read_t()) { 
        serial_write("[mouse] enable failed\r\n"); 
    } else { 
        (void)port_byte_in(0x60); // ACK
    }
    
    serial_write("[mouse] inited with 100Hz/200dpi\r\n");
}

void mouse_isr_handler(void) {
    uint8_t b = port_byte_in(0x60);
    packet[pidx++] = b;
    
    // Debug: Show that we're receiving data
    static int debug_count = 0;
    if ((debug_count++ % 100) == 0) {
        serial_write("[mouse] ISR called\r\n");
    }
    
    if (pidx >= 3) {
        pidx = 0;
        
        // Check for valid packet (bit 3 must be set in first byte)
        if (!(packet[0] & 0x08)) {
            return; // Invalid packet, ignore
        }
        
        // Parse movement deltas with proper sign extension
        int16_t dx = packet[1];
        int16_t dy = packet[2];
        
        // Handle X overflow/underflow flags
        if (packet[0] & 0x40) dx = 0; // X overflow
        if (packet[0] & 0x80) dy = 0; // Y overflow
        
        // Apply sign extension for negative movement
        if (packet[0] & 0x10) dx |= 0xFF00; // X sign bit
        if (packet[0] & 0x20) dy |= 0xFF00; // Y sign bit
        
        // Apply mouse acceleration for natural feel
        dx = apply_mouse_acceleration(dx);
        dy = apply_mouse_acceleration(dy);
        
        // Apply movement to cursor position
        // PS/2 Y is inverted (negative = up), so we negate dy
        mx += (int)dx;
        my -= (int)dy; // Note: subtract dy to match standard mouse behavior
        
        // Get screen bounds
        extern uint32_t fb_width(void); 
        extern uint32_t fb_height(void);
        int maxx = (int)fb_width() - 1; 
        int maxy = (int)fb_height() - 1;
        if (maxx < 0) maxx = 0;
        if (maxy < 0) maxy = 0;
        
        // Clamp to screen boundaries
        if (mx < 0) mx = 0; 
        if (my < 0) my = 0;
        if (mx > maxx) mx = maxx; 
        if (my > maxy) my = maxy;
        
        // Update button state (left, right, middle)
        mbtn = packet[0] & 0x07;
    }
    
    // Send EOI to both PICs
    port_byte_out(0xA0, 0x20); // Secondary PIC
    port_byte_out(0x20, 0x20); // Primary PIC
}

void mouse_get(int* x, int* y, uint8_t* buttons) {
    // Try to read any pending mouse data (polling approach as backup)
    static int poll_count = 0;
    if ((poll_count++ % 1000) == 0) {
        // Check if there's data available
        uint8_t status = port_byte_in(0x64);
        if (status & 0x01) { // Output buffer full
            if (status & 0x20) { // Mouse data available
                uint8_t data = port_byte_in(0x60);
                serial_write("[mouse] polling found data\r\n");
                // Process the data through the normal handler
                packet[pidx++] = data;
                if (pidx >= 3) {
                    pidx = 0;
                    // Process the packet like in ISR
                    if (packet[0] & 0x08) {
                        int16_t dx = packet[1];
                        int16_t dy = packet[2];
                        if (packet[0] & 0x40) dx = 0;
                        if (packet[0] & 0x80) dy = 0;
                        if (packet[0] & 0x10) dx |= 0xFF00;
                        if (packet[0] & 0x20) dy |= 0xFF00;
                        dx = apply_mouse_acceleration(dx);
                        dy = apply_mouse_acceleration(dy);
                        mx += (int)dx;
                        my -= (int)dy;
                        extern uint32_t fb_width(void); 
                        extern uint32_t fb_height(void);
                        int maxx = (int)fb_width() - 1; 
                        int maxy = (int)fb_height() - 1;
                        if (maxx < 0) maxx = 0;
                        if (maxy < 0) maxy = 0;
                        if (mx < 0) mx = 0; 
                        if (my < 0) my = 0;
                        if (mx > maxx) mx = maxx; 
                        if (my > maxy) my = maxy;
                        mbtn = packet[0] & 0x07;
                        serial_write("[mouse] polling processed packet\r\n");
                    }
                }
            }
        }
    }
    
    if (x) *x = mx; 
    if (y) *y = my; 
    if (buttons) *buttons = mbtn;
}
