HOSTCC=gcc
CFLAGS=-m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc -Wall -Wextra -O2
OBJ=boot/boot.o kernel/kernel.o kernel/terminal.o kernel/keyboard.o kernel/xcl_runtime.o kernel/xcl_generated.o
XCL_SRCS=$(wildcard src/*.xcl)
XCLC=build/xclc

all: custom-os.elf

build/xclc: tools/xclc.c
	mkdir -p build
	$(HOSTCC) -std=c99 -O2 -Wall -Wextra $< -o $@

kernel/xcl_generated.c: $(XCL_SRCS) $(XCLC)
	$(XCLC) $(XCL_SRCS) $@

boot/boot.o: boot/boot.s
	as --32 $< -o $@

kernel/xcl_generated.o: kernel/xcl_generated.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

custom-os.elf: $(OBJ) linker.ld
	ld -m elf_i386 -T linker.ld $(OBJ) -o $@
	grub-file --is-x86-multiboot $@

iso: custom-os.elf
	rm -rf iso
	mkdir -p iso/boot/grub
	cp custom-os.elf iso/boot/custom-os.elf
	cp boot/grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o custom-os.iso iso -- -volid CUSTOMOS
	test -s custom-os.iso
	xorriso -indev custom-os.iso -toc >/dev/null

uefi: custom-os.elf
	rm -rf uefi-root build/BOOTX64.EFI custom-os-uefi.img
	mkdir -p uefi-root/EFI/BOOT uefi-root/boot
	cp custom-os.elf uefi-root/boot/custom-os.elf
	grub-mkstandalone -O x86_64-efi -o build/BOOTX64.EFI "boot/grub/grub.cfg=boot/grub/grub.cfg"
	cp build/BOOTX64.EFI uefi-root/EFI/BOOT/BOOTX64.EFI
	cp boot/grub/grub.cfg uefi-root/boot/grub.cfg
	truncate -s 128M custom-os-uefi.img
	parted -s custom-os-uefi.img mklabel gpt
	parted -s custom-os-uefi.img mkpart ESP fat32 1MiB 127MiB
	parted -s custom-os-uefi.img set 1 esp on
	LOOP=$$(sudo losetup --find --show --partscan custom-os-uefi.img); \
	PART="$$LOOP"p1; \
	sudo udevadm settle; \
	sudo mkfs.fat -F 32 "$$PART" >/dev/null; \
	sudo mkdir -p /mnt/customos-esp; \
	sudo mount "$$PART" /mnt/customos-esp; \
	sudo cp -a uefi-root/. /mnt/customos-esp/; \
	sudo sync; \
	sudo umount /mnt/customos-esp; \
	sudo rmdir /mnt/customos-esp; \
	sudo losetup -d "$$LOOP"
	test -s custom-os-uefi.img
	file custom-os-uefi.img
	grub-file --is-x86_64-efi build/BOOTX64.EFI

clean:
	rm -rf iso uefi-root custom-os.elf custom-os.iso custom-os-uefi.img kernel/xcl_generated.c build