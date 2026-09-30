BITS 32
SECTION .multiboot
ALIGN 4
dd 0x1BADB002
dd 0x00000003
dd -(0x1BADB002 + 0x00000003)

SECTION .text.start
GLOBAL start
EXTERN kmain

start:
    cli
    mov esp, 0x90000
    push ebx
    push eax
    call kmain

.hang:
    hlt
    jmp .hang
