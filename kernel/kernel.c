#include "kernel.h"
#include "keyboard.h"
#include "screen.h"
#include "serial.h"

static const uint8_t WHITE_ON_BLACK = 0x0F;
static const uint8_t CYAN_ON_BLACK = 0x0B;
static const uint8_t GRAY_ON_BLACK = 0x07;

void kernel_main(void) {
    serial_init();
    clear_screen_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK);

    print_string("CUSTOM OS\n", CYAN_ON_BLACK, 2, 2);
    print_string("TEXT MODE\n", WHITE_ON_BLACK, 2, 4);
    print_string("Keyboard polling active.\n", GRAY_ON_BLACK, 2, 6);
    print_string("Type help and press Enter.\n\n> ", GRAY_ON_BLACK, 2, 8);

    keyboard_init();
    __asm__ __volatile__("cli");

    for (;;) {
        keyboard_poll();
        __asm__ __volatile__("pause");
    }
}
