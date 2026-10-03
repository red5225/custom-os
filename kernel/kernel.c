#include "kernel.h"
#include "keyboard.h"
#include "serial.h"
void kernel_main(void){
 serial_init();
 clear_screen_color(VGA_COLOR_BLACK,VGA_COLOR_BLACK);
 print_string("CUSTOM OS\n",vga_entry_color(VGA_COLOR_LIGHT_CYAN,VGA_COLOR_BLACK),2,2);
 print_string("TEXT MODE\n",vga_entry_color(VGA_COLOR_WHITE,VGA_COLOR_BLACK),2,4);
 print_string("Keyboard polling active.\n",vga_entry_color(VGA_COLOR_LIGHT_GRAY,VGA_COLOR_BLACK),2,6);
 print_string("Type help and press Enter.\n\n> ",vga_entry_color(VGA_COLOR_LIGHT_GRAY,VGA_COLOR_BLACK),2,8);
 keyboard_init();
 __asm__ __volatile__("cli");
 for(;;){keyboard_poll();__asm__ __volatile__("pause");}
}
