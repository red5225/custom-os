CC=gcc
CFLAGS=-m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc -Wall -Wextra -O2
OBJECTS=boot/boot.o kernel/kernel.o kernel/terminal.o kernel/keyboard.o kernel/bootlogo.o
all: custom-os.elf
boot/boot.o: boot/boot.s
	as --32 $< -o $@
kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@
custom-os.elf: $(OBJECTS) linker.ld
	ld -m elf_i386 -T linker.ld $(OBJECTS) -o $@
	grub-file --is-x86-multiboot2 $@
iso: custom-os.elf
	mkdir -p iso/boot/grub
	cp custom-os.elf iso/boot/custom-os.elf
	cp boot/grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o custom-os.iso iso
