HOSTCC ?= cc
CC=gcc
CFLAGS=-m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc -Wall -Wextra -O2
OBJ=boot/boot.o kernel/kernel.o kernel/terminal.o kernel/keyboard.o kernel/xcl_runtime.o kernel/xcl_generated.o
XCL_SRCS=$(wildcard src/*.xcl)
XCLC=build/xclc
all: custom-os.elf
build/xclc: tools/xclc.c
	mkdir -p build
	$(HOSTCC) -std=c99 -O2 -Wall -Wextra $< -o $@
kernel/xcl_generated.c: $(XCL_SRCS) build/xclc
	$(XCLC) $(XCL_SRCS) $@
boot/boot.o: boot/boot.s
	as --32 $< -o $@
kernel/xcl_generated.o: kernel/xcl_generated.c
	$(CC) $(CFLAGS) -c $< -o $@
kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@
custom-os.elf: $(OBJ) linker.ld
	ld -m elf_i386 -T linker.ld $(OBJ) -o $@
	grub-file --is-x86-multiboot2 $@
iso: custom-os.elf
	rm -rf iso
	mkdir -p iso/boot/grub
	cp custom-os.elf iso/boot/custom-os.elf
	cp boot/grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o custom-os.iso iso -- -as mkisofs
	test -s custom-os.iso
	xorriso -indev custom-os.iso -toc > /dev/null
clean:
	rm -rf iso custom-os.elf custom-os.iso kernel/xcl_generated.c build
