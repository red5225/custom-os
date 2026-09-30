BITS 16
ORG 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    mov [boot_drive], dl

    ; This OS is BIOS/legacy boot only. UTM SE should have UEFI disabled.
    mov ax, 0x0013
    int 0x10

    ; Load the first 16 sectors after the boot sector.
    ; The kernel is checked by the Makefile to fit in this space.
    mov ah, 0x02
    mov al, 16
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [boot_drive]
    xor bx, bx
    mov es, bx
    mov bx, 0x1000
    int 0x13
    jc disk_error

    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_mode

disk_error:
    mov si, msg
.print:
    lodsb
    test al, al
    jz .halt
    mov ah, 0x0E
    int 0x10
    jmp .print
.halt:
    hlt
    jmp .halt

BITS 32
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000
    jmp 0x10000

boot_drive db 0
msg db "Custom OS: disk read error", 0

gdt_start:
    dq 0
    dw 0xFFFF, 0
    db 0, 0x9A, 0xCF, 0
    dw 0xFFFF, 0
    db 0, 0x92, 0xCF, 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55
