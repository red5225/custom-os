.section .multiboot
.align 4
.long 0x1BADB002
.long 0
.long -(0x1BADB002 + 0)

.section .text
.global _start
.extern kmain
_start:
    cli
    mov $stack_top,%esp
    push %ebx
    push %eax
    call kmain
1:
    hlt
    jmp 1b

.section .bss
.align 16
.skip 16384
stack_top:
