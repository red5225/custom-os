typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

enum { W = 80, H = 25 };
static volatile uint16_t *const VGA = (uint16_t *)0xB8000;
static int row, col;
static uint8_t color = 0x07;

static void clear(void) {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            VGA[y * W + x] = (uint16_t)' ' | ((uint16_t)color << 8);
    row = 0;
    col = 0;
}

static void set_color(uint8_t c) { color = c; }

static void putc(char c) {
    if (c == '\n') {
        col = 0;
        row++;
        return;
    }
    if (row >= H) return;
    if (col >= W) {
        col = 0;
        row++;
        if (row >= H) return;
    }
    VGA[row * W + col] = (uint16_t)(unsigned char)c | ((uint16_t)color << 8);
    col++;
}

static void print(const char *s) {
    while (*s) putc(*s++);
}

static void line(char c) {
    for (int i = 0; i < W; i++) putc(c);
}

static void centered(const char *s) {
    int n = 0;
    while (s[n]) n++;
    col = (n < W) ? (W - n) / 2 : 0;
    print(s);
}

static void panel(const char *title, const char *value, uint8_t title_color) {
    set_color(title_color);
    print("  ");
    print(title);
    set_color(0x07);
    print(": ");
    print(value);
    print("\n");
}

void kmain(unsigned long magic, unsigned long multiboot_info) {
    (void)multiboot_info;

    clear();

    set_color(0x0B);
    line('=');
    set_color(0x0F);
    centered("CUSTOM-OS");
    set_color(0x0B);
    line('=');

    set_color(0x07);
    print("\n");
    set_color(0x0A);
    centered("SYSTEM ONLINE");
    set_color(0x07);
    print("\n\n");

    panel("STATUS", "Running", 0x0A);
    panel("KERNEL", "custom-os 0.1", 0x0B);
    panel("ARCH", "i686 / 32-bit x86", 0x0B);
    panel("DISPLAY", "VGA text console", 0x0B);
    panel("BOOT", magic == 0x2BADB002UL ? "Multiboot OK" : "Multiboot ERROR", 0x0A);

    print("\n");
    set_color(0x0E);
    print("  +----------------------------------------------------------+\n");
    print("  |                         MAIN MENU                        |\n");
    print("  +----------------------------------------------------------+\n");
    set_color(0x07);
    print("  |  [1] System information                                 |\n");
    print("  |  [2] Hardware status                                    |\n");
    print("  |  [3] Kernel console                                     |\n");
    print("  |  [4] Reboot                                             |\n");
    print("  +----------------------------------------------------------+\n");

    print("\n");
    set_color(0x08);
    print("  Waiting for keyboard input...");
    set_color(0x07);

    for (;;)
        __asm__ volatile ("hlt");
}
