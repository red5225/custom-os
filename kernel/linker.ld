/* kernel/linker.ld */
ENTRY(_start)

SECTIONS {
    /* Physical load address (1MB) */
    . = 0x00100000;

    /* Multiboot header must be in the first 8KB */
    .multiboot_header : {
        *(.multiboot_header)
    }

    /* Boot-time data (accessible before paging) */
    .bss.boot ALIGN(4K) : {
        *(.bss.boot)
    }

    .stack.boot ALIGN(16) : {
        *(.stack.boot)
    }

    /* Bootstrap code that runs with paging disabled (low identity) */
    .text.boot : {
        *(.text.boot)
    }

    /* Higher-half virtual address offset */
    . += 0xC0000000;

    /* Text section (higher half) */
    .text ALIGN (4K) : AT(ADDR(.text) - 0xC0000000) {
        *(.text)
    }

    /* Read-only data */
    .rodata ALIGN (4K) : AT(ADDR(.rodata) - 0xC0000000) {
        *(.rodata)
    }

    /* Initialized data */
    .data ALIGN (4K) : AT(ADDR(.data) - 0xC0000000) {
        *(.data)
    }

    /* Uninitialized data */
    .bss ALIGN (4K) : AT(ADDR(.bss) - 0xC0000000) {
        *(COMMON)
        *(.bss)
        *(.stack)
    }

    /* Discard these sections */
    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
    }
}