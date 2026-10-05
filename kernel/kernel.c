#include "terminal.h"
#include "keyboard.h"
#include "mouse.h"
#include "ui.h"

static void shell(void){
    terminal_write("\nCustomOS> ");
    for(;;){
        int c=keyboard_getchar();
        if(c<0)continue;
        if(c==10)terminal_write("\nCustomOS> ");
        else if(c==8)terminal_backspace();
        else terminal_putc((char)c);
    }
}
void kmain(unsigned long magic,unsigned long info){
    terminal_init();terminal_clear();
    terminal_write("================================\n");
    terminal_write("          CustomOS\n");
    terminal_write("================================\n");
    terminal_write("Custom kernel: ONLINE\n");
    terminal_write("Linux parts: separate + credited\n");
    terminal_write("Boot: Multiboot2\n");
    if(magic!=0x36d76289)terminal_write("Boot warning\n");
    ui_bind(info);
    if(ui_available())ui_run();
    keyboard_init();
    terminal_write("Framebuffer unavailable or UI closed.\n");
    shell();
}
