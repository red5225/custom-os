typedef unsigned short u16;

static volatile u16 *const VGA = (u16 *)0xB8000;

static void clear_screen(void) {
    for (int i = 0; i < 80 * 25; ++i)
        VGA[i] = (u16)' ' | ((u16)0x07 << 8);
}

static void print_at(int row, int col, const char *msg, u16 color) {
    int i = 0;
    while (msg[i]) {
        VGA[row * 80 + col + i] = (u16)msg[i] | (color << 8);
        ++i;
    }
}

void kmain(void) {
    clear_screen();
    print_at(10, 31, "HI FROM CUSTOM OS", 0x0F);
    print_at(12, 25, "minimal x86 kernel", 0x07);

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
