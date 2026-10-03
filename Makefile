CC=gcc
LD=ld
AS=nasm
OBJCOPY=objcopy
CFLAGS=-ffreestanding -Wall -Wextra -std=gnu99 -m32 -g -c -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables
LDFLAGS=-T kernel/linker.ld -m elf_i386 -nostdlib
ASFLAGS=-f elf32
KERNEL_SOURCES=kernel/kernel.c kernel/screen.c kernel/idt.c kernel/serial.c kernel/keyboard.c kernel/log.c kernel/fb.c kernel/mouse.c
KERNEL_OBJECTS=$(KERNEL_SOURCES:.c=.o)
all: custom-os.iso
wallpaper.rgb: assets/wallpaper.svg
	convert assets/wallpaper.svg -resize 640x480! -depth 8 -alpha remove -background '#0b1018' rgb:wallpaper.rgb
wallpaper.o: wallpaper.rgb
	$(OBJCOPY) -I binary -O elf32-i386 -B i386 wallpaper.rgb wallpaper.o
custom-os.iso: kernel.bin
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/kernel.bin
	cp boot/grub/grub.cfg isodir/boot/grub/
	grub-mkrescue -o $@ isodir
kernel.bin: boot.o wallpaper.o $(KERNEL_OBJECTS) kernel/interrupts.o
	$(LD) $(LDFLAGS) boot.o wallpaper.o $(KERNEL_OBJECTS) kernel/interrupts.o -o kernel.bin
	grub-file --is-x86-multiboot2 kernel.bin
boot.o: boot/boot.s
	$(AS) $(ASFLAGS) boot/boot.s -o boot.o
kernel/interrupts.o: kernel/interrupts.s
	$(AS) $(ASFLAGS) kernel/interrupts.s -o kernel/interrupts.o
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f *.o *.bin *.iso *.rgb
	rm -f kernel/*.o
	rm -rf isodir
.PHONY: all clean
