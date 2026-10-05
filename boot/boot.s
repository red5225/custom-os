.section .multiboot
.align 8
header_start:
.long 0xe85250d6
.long 0
.long header_end-header_start
.long -(0xe85250d6 + 0 + (header_end-header_start))

/* Ask GRUB for a 800x600x32 framebuffer when available. */
.align 8
.short 5
.short 0
.long 20
.long 800
.long 600
.long 32

.align 8
.short 0
.short 0
.long 8
header_end:

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
