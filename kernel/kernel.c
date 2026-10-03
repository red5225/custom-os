#include "screen.h"
#include "kernel.h"
#include "idt.h"
#include "serial.h"
#include "keyboard.h"
#include "log.h"
#include "fb.h"
#include "fb_console.h"
#include "ui.h"
#include "mouse.h"
#include "gui.h"
int g_fb_mode = 0;
int g_gui_enabled = 0;

// OS State Management
typedef enum {
    OS_STATE_WELCOME,
    OS_STATE_DESKTOP,
    OS_STATE_SHELL
} os_state_t;

static os_state_t current_state = OS_STATE_WELCOME;
static uint32_t welcome_start_time = 0;
static uint32_t last_redraw_state = 0xFF;

// Function declarations
static void draw_helixaos_logo(uint32_t center_x, uint32_t center_y);
static void draw_desktop(void);
static void draw_desktop_icon(uint32_t x, uint32_t y, const char* label, uint32_t color);
static void draw_desktop_cursor(void);
static void update_os_state(void);
static void draw_welcome_screen(void);

static int str_contains(const char* hay, const char* needle) {
    if (!hay || !needle) return 0; 
    size_t nlen = 0; 
    while (needle[nlen]) nlen++; 
    if (!nlen) return 0;
    for (size_t i = 0; hay[i]; i++) { 
        size_t j = 0; 
        while (needle[j] && hay[i+j] && hay[i+j] == needle[j]) j++; 
        if (j == nlen) return 1; 
    }
    return 0;
}

static const char* mb2_get_cmdline(uint32_t mb2_info) {
    if (!mb2_info) return 0; 
    uint8_t* p = (uint8_t*)mb2_info; 
    uint32_t total = *(uint32_t*)p; 
    (void)total; 
    p += 8;
    while (1) { 
        uint32_t type = *(uint32_t*)p; 
        uint32_t size = *(uint32_t*)(p+4); 
        if (type == 0) break; 
        if (type == 1) return (const char*)(p+8); 
        p += ((size + 7) & ~7); 
    }
    return 0;
}

// Draw a simple HelixaOS logo/icon
static void draw_helixaos_logo(uint32_t center_x, uint32_t center_y) {
    // Create a simple 32x32 pixel logo
    const uint32_t logo_size = 32;
    const uint32_t half_size = logo_size / 2;
    
    // Colors for the logo
    uint32_t primary_color = 0xFF60A5FA;   // bright blue
    uint32_t secondary_color = 0xFF3B82F6; // darker blue
    uint32_t accent_color = 0xFF06B6D4;    // cyan
    uint32_t white_color = 0xFFFFFFFF;     // white
    
    uint32_t start_x = center_x - half_size;
    uint32_t start_y = center_y - half_size;
    
    // Draw a simple geometric logo representing "AI" and "OS"
    // Outer border
    fb_rect(start_x, start_y, logo_size, 2, primary_color);
    fb_rect(start_x, start_y + logo_size - 2, logo_size, 2, primary_color);
    fb_rect(start_x, start_y, 2, logo_size, primary_color);
    fb_rect(start_x + logo_size - 2, start_y, 2, logo_size, primary_color);
    
    // Inner design - representing neural network/AI
    // Central cross pattern
    fb_rect(start_x + 14, start_y + 8, 4, 16, secondary_color);
    fb_rect(start_x + 8, start_y + 14, 16, 4, secondary_color);
    
    // Corner dots representing nodes
    fb_rect(start_x + 6, start_y + 6, 4, 4, accent_color);
    fb_rect(start_x + 22, start_y + 6, 4, 4, accent_color);
    fb_rect(start_x + 6, start_y + 22, 4, 4, accent_color);
    fb_rect(start_x + 22, start_y + 22, 4, 4, accent_color);
    
    // Central core
    fb_rect(start_x + 14, start_y + 14, 4, 4, white_color);
    
    // Connection lines
    fb_rect(start_x + 10, start_y + 10, 2, 2, accent_color);
    fb_rect(start_x + 20, start_y + 10, 2, 2, accent_color);
    fb_rect(start_x + 10, start_y + 20, 2, 2, accent_color);
    fb_rect(start_x + 20, start_y + 20, 2, 2, accent_color);
}

// Draw desktop with icons and features
static void draw_desktop(void) {
    uint32_t screen_w = fb_width(), screen_h = fb_height();
    
    // Desktop background - gradient effect
    fb_fill(0xFF1E293B); // Dark slate background
    
    // Top bar
    fb_rect(0, 0, screen_w, 40, 0xFF334155);
    fb_rect(0, 38, screen_w, 2, 0xFF60A5FA);
    
    // OS Title in top bar
    fb_draw_string_scaled(20, 12, "HelixaOS Desktop", 0xFFFFFFFF, 0xFF334155, 2);
    
    // Clock area (placeholder)
    fb_draw_string(screen_w - 120, 12, "00:00:00", 0xFFE5E7EB, 0xFF334155);
    
    // Desktop icons grid
    draw_desktop_icon(100, 80, "Terminal", 0xFF10B981);
    draw_desktop_icon(250, 80, "Files", 0xFF3B82F6);
    draw_desktop_icon(400, 80, "Settings", 0xFF8B5CF6);
    draw_desktop_icon(550, 80, "About", 0xFFF59E0B);
    
    draw_desktop_icon(100, 200, "Calculator", 0xFFEF4444);
    draw_desktop_icon(250, 200, "Text Editor", 0xFF06B6D4);
    draw_desktop_icon(400, 200, "AI Chat", 0xFF84CC16);
    draw_desktop_icon(550, 200, "Games", 0xFFEC4899);
    
    // Status bar at bottom
    fb_rect(0, screen_h - 30, screen_w, 30, 0xFF1F2937);
    fb_rect(0, screen_h - 32, screen_w, 2, 0xFF60A5FA);
    fb_draw_string(20, screen_h - 20, "Ready | Memory: 128MB | AI OS v1.0", 0xFFE5E7EB, 0xFF1F2937);
    
    // Draw mouse cursor
    draw_desktop_cursor();
}

// Draw simple mouse cursor for desktop
static void draw_desktop_cursor(void) {
    static int test_x = 400, test_y = 300;
    static int test_dx = 2, test_dy = 1;
    static int test_counter = 0;
    
    // Move cursor in a bouncing pattern for testing
    if ((test_counter++ % 5) == 0) {
        test_x += test_dx;
        test_y += test_dy;
        
        // Bounce off screen edges
        if (test_x <= 10 || test_x >= 1000) test_dx = -test_dx;
        if (test_y <= 50 || test_y >= 700) test_dy = -test_dy;
    }
    
    int mx = test_x, my = test_y;
    uint8_t buttons = 0;
    
    // Original mouse reading (commented out for testing)
    // mouse_get(&mx, &my, &buttons);
    
    // Debug: Add a simple test to see if mouse coordinates are changing
    static int debug_counter = 0;
    if ((debug_counter++ % 50) == 0) {
        // Simple debug - just show that function is being called
        serial_write("[cursor] drawing at test position\r\n");
    }
    
    // Ensure cursor is within bounds
    uint32_t screen_w = fb_width(), screen_h = fb_height();
    if (mx < 0) mx = 0;
    if (my < 0) my = 0;
    if (mx >= (int)screen_w - 12) mx = (int)screen_w - 12;
    if (my >= (int)screen_h - 12) my = (int)screen_h - 12;
    
    // Simple arrow cursor (white with black outline)
    // Draw black outline first
    for (int y = 0; y < 12; y++) {
        for (int x = 0; x < 8; x++) {
            if ((x == 0 && y < 10) ||           // Left edge
                (y == 0 && x < 7) ||            // Top edge  
                (x == y && y < 7) ||            // Diagonal
                (x == 3 && y >= 7 && y < 10) || // Vertical part
                (y == 9 && x >= 3 && x < 6)) {  // Bottom
                fb_putpixel(mx + x, my + y, 0xFF000000); // Black outline
            }
        }
    }
    
    // Draw white interior
    for (int y = 1; y < 11; y++) {
        for (int x = 1; x < 7; x++) {
            if ((x <= y && y < 6) ||            // Upper triangle
                (x == 4 && y >= 7 && y < 9)) {  // Vertical part
                fb_putpixel(mx + x, my + y, 0xFFFFFFFF); // White interior
            }
        }
    }
}

// Draw a desktop icon with label
static void draw_desktop_icon(uint32_t x, uint32_t y, const char* label, uint32_t color) {
    // Icon background
    fb_rect(x, y, 64, 64, color);
    fb_rect(x + 2, y + 2, 60, 60, 0xFF1E293B);
    
    // Icon border
    fb_rect(x, y, 64, 2, 0xFF60A5FA);
    fb_rect(x, y + 62, 64, 2, 0xFF60A5FA);
    fb_rect(x, y, 2, 64, 0xFF60A5FA);
    fb_rect(x + 62, y, 2, 64, 0xFF60A5FA);
    
    // Simple icon design in center
    fb_rect(x + 20, y + 20, 24, 24, color);
    fb_rect(x + 24, y + 24, 16, 16, 0xFFFFFFFF);
    
    // Label below icon
    uint32_t label_len = 0;
    while (label[label_len]) label_len++;
    
    // Calculate centered position with bounds checking
    uint32_t text_width = label_len * 8;
    uint32_t label_x;
    if (text_width <= 64) {
        label_x = x + (64 - text_width) / 2;
    } else {
        // If text is too wide, position it to start at icon left edge
        label_x = x;
    }
    
    fb_draw_string(label_x, y + 70, label, 0xFFFFFFFF, 0xFF1E293B);
}

// Update OS state based on timer
static void update_os_state(void) {
    if (current_state == OS_STATE_WELCOME) {
        uint32_t current_time = timer_ticks();
        uint32_t elapsed = current_time - welcome_start_time;
        
        // Check if 5 seconds (500 ticks at 100Hz) have passed
        if (elapsed >= 500) {
            current_state = OS_STATE_DESKTOP;
            serial_write("[os] transitioning to desktop\r\n");
        }
    }
}

// Draw the welcome screen
static void draw_welcome_screen(void) {
    // High-contrast clean welcome screen
    fb_fill(0xFF0F1629); // Very dark blue background for maximum contrast
    
    // High contrast colors for crystal clear text
    uint32_t bg_color = 0xFF0F1629; // very dark blue background
    uint32_t title_color = 0xFFFFFFFF; // pure white for title
    uint32_t subtitle_color = 0xFFE5E7EB; // very light gray for subtitle
    uint32_t accent_color = 0xFF60A5FA; // bright blue accent
    uint32_t version_color = 0xFFA3A3A3; // medium gray for version
    
    // Text content
    const char* title = "HelixaOS";
    const char* subtitle = "AI Operating System";
    const char* version = "v1.0";
    const char* loading = "Loading desktop...";
    
    uint32_t title_scale = 4; // Larger title for better visibility
    uint32_t subtitle_scale = 2; // Medium subtitle
    
    // Calculate dimensions
    uint32_t title_w = 8 * title_scale, title_h = 8 * title_scale;
    uint32_t subtitle_w = 8 * subtitle_scale, subtitle_h = 8 * subtitle_scale;
    
    // String lengths
    uint32_t title_len = 0; while (title[title_len]) title_len++;
    uint32_t subtitle_len = 0; while (subtitle[subtitle_len]) subtitle_len++;
    uint32_t version_len = 0; while (version[version_len]) version_len++;
    uint32_t loading_len = 0; while (loading[loading_len]) loading_len++;
    
    // Screen center
    uint32_t screen_w = fb_width(), screen_h = fb_height();
    uint32_t center_x = screen_w / 2, center_y = screen_h / 2;
    
    // Position elements with generous spacing for clarity
    uint32_t title_x = center_x - (title_len * title_w) / 2;
    uint32_t title_y = center_y - 80;
    
    uint32_t subtitle_x = center_x - (subtitle_len * subtitle_w) / 2;
    uint32_t subtitle_y = title_y + title_h + 24;
    
    uint32_t version_x = center_x - (version_len * 8) / 2;
    uint32_t version_y = subtitle_y + subtitle_h + 32;
    
    uint32_t loading_x = center_x - (loading_len * 8) / 2;
    uint32_t loading_y = version_y + 50;
    
    // Draw accent elements for visual hierarchy
    fb_rect(center_x - 60, title_y - 20, 120, 3, accent_color);
    
    // Render text with enhanced visibility
    fb_draw_string_scaled(title_x, title_y, title, title_color, bg_color, title_scale);
    fb_draw_string_scaled(subtitle_x, subtitle_y, subtitle, subtitle_color, bg_color, subtitle_scale);
    fb_draw_string(version_x, version_y, version, version_color, bg_color);
    fb_draw_string(loading_x, loading_y, loading, accent_color, bg_color);
    
    // Bottom accent line
    fb_rect(center_x - 60, version_y + 24, 120, 3, accent_color);
    
    // Draw a small logo/image below the welcome text
    draw_helixaos_logo(center_x, version_y + 80);
    
    // Progress bar
    uint32_t progress_w = 200;
    uint32_t progress_x = center_x - progress_w / 2;
    uint32_t progress_y = loading_y + 30;
    
    fb_rect(progress_x, progress_y, progress_w, 6, 0xFF374151);
    
    // Calculate progress based on actual timer
    uint32_t elapsed_time = timer_ticks() - welcome_start_time;
    uint32_t progress_fill = (elapsed_time * progress_w) / 500; // 500 ticks = 5 seconds
    if (progress_fill > progress_w) progress_fill = progress_w;
    fb_rect(progress_x, progress_y, progress_fill, 6, accent_color);
}

void kernel_main(void) {
    #ifndef DEBUG
    #define DEBUG 1
    #endif
    klog_init();
    serial_init();
    #if DEBUG
    serial_write("[k] booting\\r\\n");
    serial_write("[k] drawing ui...\\r\\n");
    #endif
    // Toggle this to 1 to re-enable interrupts during debugging
    #ifndef ENABLE_IRQS
    #define ENABLE_IRQS 1
    #endif
    #if ENABLE_IRQS
        #if DEBUG
        serial_write("[k] idt init...\\r\\n");
        #endif
        idt_init();
        #if DEBUG
        serial_write("[k] pit init 100Hz...\\r\\n");
        #endif
        pit_init(100);
    #endif

    // Detect GRUB cmdline mode=fb|text to make modes exclusive
    const char* cmd = mb2_get_cmdline(mb2_info_addr);
    if (cmd) {
        if (str_contains(cmd, "mode=fb")) g_fb_mode = 1; else if (str_contains(cmd, "mode=text")) g_fb_mode = 0;
    }

    disable_cursor();
    if (g_fb_mode) {
        #if DEBUG
        serial_write("[fb] init from multiboot...\r\n");
        #endif
        if (fb_init_from_multiboot(mb2_info_addr) && fb_is_available()) {
            #if DEBUG
            serial_write("[fb] init ok\r\n");
            #endif
        } else {
            // Fallback to text if FB missing
            #if DEBUG
            serial_write("[fb] not available, falling back to text\r\n");
            #endif
            g_fb_mode = 0;
        }
    }

    if (!g_fb_mode) {
        // Text mode UI
        clear_screen_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLUE);
    }

    // Header and status bar (draw inside the border) - text mode only
    if (!g_fb_mode) {
        uint8_t bar_bg = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
        uint8_t bar_fg = vga_entry_color(VGA_COLOR_YELLOW, VGA_COLOR_BLUE);
        fill_row(1, bar_bg);
        fill_row(MAX_ROWS - 2, bar_bg);
        const char *title = " HelixaOS v1.0 ";
        int tlen = 0; while (title[tlen]) tlen++;
        size_t tx = (tlen < MAX_COLS) ? (MAX_COLS - tlen) / 2 : 0;
        print_string(title, bar_fg, tx, 1);
        const char *status = " [Uptime] VGA text mode ";
        int status_len = 0; while (status[status_len]) status_len++;
        size_t status_x = (status_len < MAX_COLS) ? (MAX_COLS - status_len - 2) : 2;
        print_string(status, bar_fg, status_x, MAX_ROWS - 2);
        draw_border(vga_entry_color(VGA_COLOR_YELLOW, VGA_COLOR_BLUE));
        size_t box_w = 46, box_h = 7; size_t box_x = (MAX_COLS - box_w) / 2; size_t box_y = (MAX_ROWS - box_h) / 2;
        draw_box(box_x, box_y, box_w, box_h, vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLUE), vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_BLUE));
        print_centered("Welcome to HelixaOS", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE), box_y + 2);
        print_centered("Designed for AI", vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLUE), box_y + 4);
    } else {
        // Framebuffer mode: State-based UI system
        
        // Enable mouse and GUI for desktop interaction
        g_gui_enabled = 1;
        
        // Initialize mouse if available
        #if ENABLE_IRQS
        if (fb_is_available()) {
            mouse_init();
        }
        #endif
        
        // Initial welcome screen
        welcome_start_time = timer_ticks(); // Record start time
        serial_write("[os] welcome screen started\r\n");
        draw_welcome_screen();
    }

    // Prepare keyboard input (text UI only)
    if (!g_fb_mode) keyboard_init();

    #if ENABLE_IRQS
        #if DEBUG
        serial_write("[k] enabling interrupts (sti)\\r\\n");
        #endif
        __asm__ __volatile__("sti");
    #endif
    
    // Main loop with state management
    static int last_mx = -1, last_my = -1;
    
    while (1) {
        if (g_fb_mode) {
            update_os_state();
            uint32_t current_time = timer_ticks();
            
            // Check if mouse moved
            int mx, my;
            uint8_t buttons;
            mouse_get(&mx, &my, &buttons);
            int mouse_moved = (mx != last_mx || my != last_my);
            
            // Debug: Show mouse coordinates periodically
            static int mouse_debug_count = 0;
            if ((mouse_debug_count++ % 100) == 0) {
                serial_write("[main] mouse position: ");
                // Simple decimal output for debugging
                char coords[32];
                coords[0] = '0' + (mx / 100) % 10;
                coords[1] = '0' + (mx / 10) % 10;
                coords[2] = '0' + mx % 10;
                coords[3] = ',';
                coords[4] = '0' + (my / 100) % 10;
                coords[5] = '0' + (my / 10) % 10;
                coords[6] = '0' + my % 10;
                coords[7] = '\r';
                coords[8] = '\n';
                coords[9] = '\0';
                serial_write(coords);
            }
            
            // Redraw based on state with smart update logic
            switch (current_state) {
                case OS_STATE_WELCOME:
                    // Welcome screen with progress bar updates
                    if (current_state != last_redraw_state || (current_time % 10) == 0) {
                        draw_welcome_screen();
                    }
                    break;
                    
                case OS_STATE_DESKTOP:
                    // Desktop updates only when needed (more responsive)
                    if (current_state != last_redraw_state || mouse_moved) {
                        draw_desktop();
                        last_mx = mx;
                        last_my = my;
                    }
                    break;
                    
                case OS_STATE_SHELL:
                    // Shell window will be implemented next
                    break;
            }
            last_redraw_state = current_state;
        }
        __asm__ __volatile__("hlt");
    }
}

uint8_t port_byte_in(uint16_t port) {
    uint8_t result;
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "dN"(port));
    return result;
}

void port_byte_out(uint16_t port, uint8_t data) {
    __asm__ __volatile__("outb %0, %1" :: "a"(data), "dN"(port));
}