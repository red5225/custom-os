# Makefile
ARCH=i386-elf
CC=$(ARCH)-gcc
AS=nasm
LD=$(ARCH)-ld

# Build mode: DEBUG=1 (default). Use `make RELEASE=1` for quiet serial.
DEBUG?=1
ifeq ($(RELEASE),1)
	CDEFS+=-DRELEASE
	DEBUG=0
endif

CFLAGS=-ffreestanding -Wall -Wextra -std=gnu99 -m32 -g -c $(CDEFS)
LDFLAGS=-T kernel/linker.ld -m elf_i386 -nostdlib
ASFLAGS=-f elf32

KERNEL_SOURCES=kernel/kernel.c kernel/screen.c kernel/idt.c kernel/serial.c kernel/keyboard.c kernel/log.c kernel/fb.c kernel/mouse.c kernel/gui.c kernel/fb_console.c kernel/ui_fb.c
KERNEL_OBJECTS=$(KERNEL_SOURCES:.c=.o)

all: myos.iso

myos.iso: kernel.bin
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/kernel.bin
	cp boot/grub/grub.cfg isodir/boot/grub/
	grub-mkrescue -o myos.iso isodir

kernel.bin: boot.o $(KERNEL_OBJECTS) kernel/interrupts.o
	$(LD) $(LDFLAGS) boot.o $(KERNEL_OBJECTS) kernel/interrupts.o -o kernel.bin

boot.o: boot/boot.s
	$(AS) $(ASFLAGS) boot/boot.s -o boot.o

kernel/interrupts.o: kernel/interrupts.s
	$(AS) $(ASFLAGS) kernel/interrupts.s -o kernel/interrupts.o

kernel.bin: kernel/interrupts.o

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o *.bin *.iso
	rm -f kernel/*.o
	rm -rf isodir

run: myos.iso
	@export DISPLAY=:0; qemu-system-i386 -vga std -cdrom myos.iso

run-serial: myos.iso
	@export DISPLAY=:0; qemu-system-i386 -vga std -cdrom myos.iso -serial stdio -no-reboot -no-shutdown

.PHONY: all clean run