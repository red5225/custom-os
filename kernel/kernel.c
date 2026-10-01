typedef unsigned short u16;
static volatile u16 *const VGA = (u16 *)0xB8000;
void kmain(void) {
    const char *msg = "HI FROM CUSTOM OS";
    for (int i = 0; msg[i]; ++i)
        VGA[i] = (u16)msg[i] | ((u16)0x0F << 8);
}
