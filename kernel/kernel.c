#include "terminal.h"
#include "keyboard.h"
static void shell(void){terminal_write("\nCustomOS> ");for(;;){int c=keyboard_getchar();if(c<0)continue;if(c==10)terminal_write("\nCustomOS> ");else if(c==8)terminal_backspace();else terminal_putc((char)c);}}
void kmain(unsigned long magic,unsigned long info){(void)info;terminal_init();terminal_clear();terminal_write("========================================\n              CustomOS\n========================================\nCustom kernel: ONLINE\nLinux parts: separated + credited\nBoot: Multiboot2\n----------------------------------------\n");if(magic!=0x36d76289)terminal_write("Boot warning\n");keyboard_init();terminal_write("Terminal ready.\n");shell();}
