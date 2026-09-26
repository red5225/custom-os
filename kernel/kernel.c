typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

enum { VGA_WIDTH = 80, VGA_HEIGHT = 25 };
static volatile uint16_t *const VGA = (uint16_t *)0xB8000;
static int row = 0;
static int col = 0;

static void clear(void) {
    for (int y = 0; y < VGA_HEIGHT; y++)
        for (int x = 0; x < VGA_WIDTH; x++)
            VGA[y * VGA_WIDTH + x] = (uint16_t)' ' | (uint16_t)(0x07 << 8);
    row = 0;
    col = 0;
}

static void putc(char c) {
    if (c == '\n') {
        col = 0;
        row++;
    } else {
        if (row < VGA_HEIGHT && col < VGA_WIDTH)
            VGA[row * VGA_WIDTH + col] =
                (uint16_t)(unsigned char)c | (uint16_t)(0x07 << 8);
        col++;
        if (col >= VGA_WIDTH) {
            col = 0;
            row++;
        }
    }

    if (row >= VGA_HEIGHT)
        row = 0;
}

static void print(const char *s) {
    while (*s)
        putc(*s++);
}

void kmain(unsigned long magic, unsigned long multiboot_info) {
    (void)multiboot_info;

    clear();
    print("CUSTOM-OS\n");
    print("========================================\n");
    print("32-bit x86 kernel is running.\n");
    print("Multiboot magic: ");

    if (magic == 0x2BADB002UL)
        print("OK\n");
    else
        print("INVALID\n");

    print("\nUTM VGA console is active.\n");
    print("Boot completed successfully.\n");

    for (;;)
        __asm__ volatile ("hlt");
}
