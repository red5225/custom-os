#include "kernel.h"
#include "keyboard.h"
#include "serial.h"
void kernel_main(void){serial_init();clear_screen_color(VGA_COLOR_BLACK,VGA_COLOR_BLACK);print_string("CUSTOM OS\n\nText-only mode\nChromebook/UTM friendly build\n\nKeyboard polling active.\nType commands below.\n\n> ",vga_entry_color(VGA_COLOR_LIGHT_GRAY,VGA_COLOR_BLACK),2,2);keyboard_init();__asm__ __volatile__("cli");for(;;){keyboard_poll();__asm__ __volatile__("pause");}}
