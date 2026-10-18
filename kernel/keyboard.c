#include "keyboard.h"

#include <stdint.h>

static const char normal[128] = {
    [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',[0x06]='5',[0x07]='6',[0x08]='7',[0x09]='8',[0x0A]='9',[0x0B]='0',
    [0x0C]='-',[0x0D]='=',[0x10]='q',[0x11]='w',[0x12]='e',[0x13]='r',[0x14]='t',[0x15]='y',[0x16]='u',[0x17]='i',[0x18]='o',[0x19]='p',
    [0x1A]='[',[0x1B]=']',[0x1C]='\n',[0x1E]='a',[0x1F]='s',[0x20]='d',[0x21]='f',[0x22]='g',[0x23]='h',[0x24]='j',[0x25]='k',[0x26]='l',
    [0x27]=';',[0x28]='\'',[0x29]='`',[0x2B]='\\',[0x2C]='z',[0x2D]='x',[0x2E]='c',[0x2F]='v',[0x30]='b',[0x31]='n',[0x32]='m',
    [0x33]=',',[0x34]='.',[0x35]='/',[0x39]=' '
};

static uint8_t inb(uint16_t port) { uint8_t v; __asm__ volatile("inb %1,%0" : "=a"(v) : "Nd"(port)); return v; }
static void outb(uint16_t port, uint8_t v) { __asm__ volatile("outb %0,%1" :: "a"(v), "Nd"(port)); }
static int wait_input_clear(void) { for (uint32_t i=0;i<100000u;i++) if (!(inb(0x64)&2u)) return 1; return 0; }
static int wait_output(void) { for (uint32_t i=0;i<100000u;i++) if (inb(0x64)&1u) return 1; return 0; }
static int send_keyboard(uint8_t value) {
    if (!wait_input_clear()) return 0;
    outb(0x60, value);
    if (!wait_output()) return 0;
    return inb(0x60) == 0xFA;
}

void keyboard_init(void) {
    if (wait_input_clear()) outb(0x64, 0xAD); /* disable keyboard while draining stale data */
    for (uint32_t i=0;i<256u && (inb(0x64)&1u);i++) (void)inb(0x60);
    if (wait_input_clear()) outb(0x64, 0xAE);
    (void)send_keyboard(0xF0);
    (void)send_keyboard(0x01); /* set 1 scan codes */
    (void)send_keyboard(0xF4); /* enable scanning */
}

int keyboard_getchar(void) {
    uint8_t status=inb(0x64);
    if (!(status&1u) || (status&0x20u)) return -1; /* do not eat mouse bytes */
    uint8_t scan=inb(0x60);
    if ((scan&0x80u) || scan>=128u) return -1;
    return normal[scan] ? (unsigned char)normal[scan] : -1;
}
