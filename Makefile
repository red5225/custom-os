CC=gcc
LD=ld
AS=nasm

CFLAGS=-ffreestanding -Wall -Wextra -std=gnu99 -m32 -g -c -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables
LDFLAGS=-T kernel/linker.ld -m elf_i386 -nostdlib
ASFLAGS=-f elf32

KERNEL_SOURCES=kernel/kernel.c kernel/screen.c kernel/serial.c kernel/keyboard.c kernel/log.c kernel/mouse.c
KERNEL_OBJECTS=$(KERNEL_SOURCES:.c=.o)

all: custom-os.iso

custom-os.iso: kernel.bin
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/kernel.bin
	cp boot/grub/grub.cfg isodir/boot/grub/
	grub-mkrescue -o $@ isodir

kernel.bin: boot.o $(KERNEL_OBJECTS)
	$(LD) $(LDFLAGS) boot.o $(KERNEL_OBJECTS) -o kernel.bin
	grub-file --is-x86-multiboot2 kernel.bin

boot.o: boot/boot.s
	$(AS) $(ASFLAGS) boot/boot.s -o boot.o

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o *.bin *.iso
	rm -f kernel/*.o
	rm -rf isodir

.PHONY: all clean
