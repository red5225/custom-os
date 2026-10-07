#include "terminal.h"
#include "keyboard.h"

extern void boot(void);
extern void command(const char *cmd);
extern void prompt(void);

static void shell(void){
    char line[128];
    unsigned int len=0;
    prompt();
    for(;;){
        int c=keyboard_getchar();
        if(c<0) continue;
        if(c==10){
            line[len]=0;
            command(line);
            len=0;
            continue;
        }
        if(c==8){
            if(len){ --len; terminal_backspace(); }
            continue;
        }
        if(c>=32 && c<127 && len<sizeof(line)-1){
            line[len++]=(char)c;
            terminal_putc((char)c);
        }
    }
}

void kmain(unsigned long magic,unsigned long info){
    (void)info;
    terminal_init();
    terminal_clear();
    boot();
    if(magic!=0x36d76289) terminal_write("Boot warning: unexpected Multiboot2 magic.\n");
    keyboard_init();
    shell();
}
