BITS 32
SECTION .text.start
GLOBAL start
EXTERN kmain

start:
    cli
    mov esp, 0x90000
    call kmain

.hang:
    hlt
    jmp .hang
