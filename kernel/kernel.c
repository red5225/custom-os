#include "kernel.h"
#include "keyboard.h"
#include "mouse.h"
#include "usb_watch.h"
#include "screen.h"
#include "serial.h"

static const uint8_t WHITE_ON_BLACK=0x0F;
static const uint8_t CYAN_ON_BLACK=0x0B;
static const uint8_t GRAY_ON_BLACK=0x07;

void kernel_main(void) {
    serial_init();
    clear_screen_color(0,0);
    print_string("CUSTOM OS\n",CYAN_ON_BLACK,2,2);
    print_string("TEXT MODE\n",WHITE_ON_BLACK,2,4);
    print_string("Keyboard + PS/2 mouse polling active.\n",GRAY_ON_BLACK,2,6);
    print_string("USB removal watchdog armed when xHCI sees a USB device.\n",GRAY_ON_BLACK,2,7);
    print_string("Type help and press Enter.\n\n> ",GRAY_ON_BLACK,2,9);
    keyboard_init();
    mouse_init();
    usb_watch_init();
    __asm__ volatile("cli");
    for(;;) {
        keyboard_poll();
        mouse_poll();
        usb_watch_poll();
        __asm__ volatile("pause");
    }
}
