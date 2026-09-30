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

    ; Set 320x200 256-color VGA mode.
    mov ax, 0x0013
    int 0x10

    ; Read the kernel from LBA 1 using BIOS INT 13h extensions.
    ; This works with UTM's El Torito CD/ISO boot drive as well as
    ; normal BIOS disk emulation.
    mov si, disk_address_packet
    mov dl, [boot_drive]
    mov ah, 0x42
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

align 4
disk_address_packet:
    db 0x10
    db 0
    dw 16
    dw 0x1000
    dw 0
    dq 1

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
