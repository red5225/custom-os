[bits 32]
global isr_stub_irq0
global isr_stub_irq1
global isr_stub_irq12

; Exception stubs 0..31
%macro ISR_NOERR 1
global isr%1
isr%1:
    push dword 0           ; fake error code
    push dword %1          ; interrupt number
    pusha
    push gs
    push fs
    push es
    push ds
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp               ; pointer to registers struct
    extern fault_isr_handler
    call fault_isr_handler
    add esp, 4
    add esp, 16            ; pop ds/es/fs/gs
    popa
    add esp, 8             ; pop int_no and err_code
    iretd
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    push dword %1          ; interrupt number (CPU already pushed err code)
    pusha
    push gs
    push fs
    push es
    push ds
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp
    extern fault_isr_handler
    call fault_isr_handler
    add esp, 4
    add esp, 16
    popa
    add esp, 8             ; pop int_no and err_code
    iretd
%endmacro

; Generate stubs
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

; Existing IRQ stubs
extern timer_isr_handler
extern keyboard_isr_handler
extern mouse_isr_handler

isr_stub_irq0:
    pusha
    cld
    call timer_isr_handler
    popa
    iretd

isr_stub_irq1:
    pusha
    cld
    call keyboard_isr_handler
    popa
    iretd

isr_stub_irq12:
    pusha
    cld
    call mouse_isr_handler
    popa
    iretd