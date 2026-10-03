[bits 32]

[bits 32]

section .multiboot_header
header_start:
    dd 0xe85250d6                ; Magic
    dd 0                         ; Architecture
    dd header_end - header_start ; Length
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start)) ; Checksum
    align 8
    ; Multiboot2 Framebuffer request tag (type=5)
    dw 5                         ; Tag type: framebuffer
    dw 0                         ; Flags
    dd 20                        ; Size of this tag (bytes)
    dd 1024                      ; Width
    dd 768                       ; Height
    dd 32                        ; Depth (bpp)
    align 8
    ; End tag
    dw 0
    dw 0
    dd 8
header_end:

; Early boot code runs without paging (identity-mapped)
section .text.boot
global _start
_start:
    ; Save Multiboot2 info pointer (EBX) from GRUB
    mov [mb2_info_addr], ebx
    mov esp, stack_top_phys      ; Temporary stack
    cli                          ; ensure interrupts off during setup

    call setup_paging            ; Initialize page tables
    mov eax, cr0
    or eax, 0x80000000           ; Enable paging
    mov cr0, eax
    jmp higher_half_entry

setup_paging:
    ; Clear page directory
    mov edi, page_directory
    xor eax, eax
    mov ecx, 1024
    rep stosd

    ; Map first 12MB (identity): PDE[0]=0..4MB, PDE[1]=4..8MB, PDE[2]=8..12MB
    mov eax, page_table_0
    or eax, 0x003
    mov [page_directory], eax
    mov eax, page_table_k1
    or eax, 0x003
    mov [page_directory + 1*4], eax
    mov eax, page_table_k2
    or eax, 0x003
    mov [page_directory + 2*4], eax

    ; Map higher-half kernel: 0xC0000000..0xC0BFFFFF (first 12MB physical)
    mov eax, page_table_0
    or eax, 0x003
    mov [page_directory + 768*4], eax         ; 0..4MB via page_table_0
    mov eax, page_table_k1
    or eax, 0x003
    mov [page_directory + 769*4], eax         ; 4..8MB via page_table_k1
    mov eax, page_table_k2
    or eax, 0x003
    mov [page_directory + 770*4], eax         ; 8..12MB via page_table_k2

    ; Map VGA memory (0xB8000 -> 0xC00B8000)
    mov edi, page_table_0 + (0xB8 * 4)  ; 0xB8000 / 4096 = 0xB8
    mov eax, 0xB8000 | 0x003
    mov [edi], eax

    ; Fill page table (first 4MB)

        ; Pre-paging: clear screen and print a boot banner to 0xB8000
        mov edi, 0xB8000             ; VGA text buffer (physical/identity)
        mov ax, 0x0F20               ; ' ' with white-on-black attribute
        mov ecx, 80*25
    .clear_screen_early:
        mov [edi], ax
        add edi, 2
        loop .clear_screen_early

        ; Write "Booting MyOS..." at top-left
        mov edi, 0xB8000
        mov esi, boot_msg
        mov bl, 0x0F                 ; white on black
    .print_boot:
        lodsb
        test al, al
        jz .after_boot_print
        mov ah, bl
        stosw
        jmp .print_boot
    .after_boot_print:
    mov edi, page_table_0
    mov eax, 0x00000003  ; Present + RW
    mov ecx, 1024
.fill_table0:
    mov [edi], eax
    add eax, 0x1000
    add edi, 4
    loop .fill_table0

    mov edi, page_table_k1
    mov eax, 0x00400000 | 0x00000003  ; 4MB start
    mov ecx, 1024
.fill_table1:
    mov [edi], eax
    add eax, 0x1000
    add edi, 4
    loop .fill_table1

    mov edi, page_table_k2
    mov eax, 0x00800000 | 0x00000003  ; 8MB start
    mov ecx, 1024
.fill_table2:
    mov [edi], eax
    add eax, 0x1000
    add edi, 4
    loop .fill_table2

    mov eax, page_directory
    mov cr3, eax
    ret

; Switch to higher-half code and stack
section .text
higher_half_entry:
    mov esp, stack_top_kernel    ; Higher-half stack

    ; Call kernel main
    extern kernel_main
    call kernel_main

    cli
.hang:
    hlt
    jmp .hang

; Boot string stored in low memory with code (pre-paging)
section .text.boot
boot_msg: db 'Booting MyOS...', 0

; Early boot data (low, identity-mapped)
section .bss.boot nobits
align 4096
global page_directory
global page_table_0
global page_table_k1
global page_table_k2
global page_table_fb0
global page_table_fb1
global mb2_info_addr
page_directory:   resb 4096
page_table_0:     resb 4096
page_table_k1:    resb 4096
page_table_k2:    resb 4096
page_table_fb0:   resb 4096
page_table_fb1:   resb 4096
mb2_info_addr:    resd 1

section .stack.boot nobits align=16
stack_bottom_phys:
    resb 16384  ; 16KB temp stack
stack_top_phys:

; Kernel stack in higher-half
section .stack nobits align=16
stack_bottom_kernel:
    resb 16384  ; 16KB kernel stack
stack_top_kernel: