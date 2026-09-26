typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

enum { VGA_WIDTH = 80, VGA_HEIGHT = 25 };
static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static void clear(void) {
    for (int y = 0; y < VGA_HEIGHT; y++)
        for (int x = 0; x < VGA_WIDTH; x++)
            VGA[y * VGA_WIDTH + x] = (uint16_t)' ' | (uint16_t)(0x07 << 8);
}

static void print(const char *s) {
    static int pos = 0;
    while (*s && pos < VGA_WIDTH * VGA_HEIGHT) {
        VGA[pos++] = (uint16_t)*s++ | (uint16_t)(0x07 << 8);
    }
}

void kmain(unsigned long magic, unsigned long multiboot_info) {
    (void)magic;
    (void)multiboot_info;
    clear();
    print("custom-os booted successfully!\n");
    print("32-bit x86 kernel running under UTM.\n");
}
