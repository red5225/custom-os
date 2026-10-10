#include <stdint.h>
#include <stddef.h>

struct boot_info {
    uint64_t magic;
    uint64_t memory_map_size;
    uint64_t memory_map_key;
    uint64_t descriptor_size;
    uint32_t descriptor_version;
    uint32_t reserved;
    void *memory_map;
};

#define BOOT_INFO_MAGIC UINT64_C(0x4E4F4C424F4F5431)

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static void serial_init(void) {
    outb(0x3F9, 0x00);
    outb(0x3FB, 0x80);
    outb(0x3F8, 0x01);
    outb(0x3F9, 0x00);
    outb(0x3FB, 0x03);
    outb(0x3FA, 0xC7);
    outb(0x3FC, 0x0B);
}

static void serial_puts(const char *s) {
    while (*s) {
        if (*s == '\n') outb(0x3F8, '\r');
        outb(0x3F8, (uint8_t)*s++);
    }
}

__attribute__((noreturn)) void kernel_entry(struct boot_info *info) {
    serial_init();
    serial_puts("\nNOL OS MILESTONE 1: kernel reached\n");
    if (info && info->magic == BOOT_INFO_MAGIC) {
        serial_puts("Boot info: valid; UEFI boot services have ended.\n");
    } else {
        serial_puts("Boot info: invalid.\n");
    }
    serial_puts("Kernel halted safely.\n");
    for (;;) __asm__ volatile ("cli; hlt");
}
