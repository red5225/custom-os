#include "terminal.h"
#include "keyboard.h"
#include "bootlogo.h"
static void shell(void){terminal_write("CustomOS> ");for(;;){int c=keyboard_getchar();if(c<0)continue;if(c==10)terminal_write("\nCustomOS> ");else if(c==8)terminal_backspace();else terminal_putc((char)c);}}
void kmain(unsigned long magic,unsigned long info){(void)magic;(void)info;terminal_init();bootlogo_show();keyboard_init();terminal_write("Terminal ready.\n");shell();}
