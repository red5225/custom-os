TARGET := i686-elf
BUILD := build

CFLAGS := -m32 -ffreestanding -fno-pie -fno-stack-protector -Wall -Wextra -O2
LDFLAGS := -m elf_i386 -T linker.ld

all: $(BUILD)/custom-os.iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: boot/boot.asm | $(BUILD)
	nasm -f elf32 $< -o $@

$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD)/custom-os.bin: $(BUILD)/boot.o $(BUILD)/kernel.o linker.ld
	ld $(LDFLAGS) -o $@ $(BUILD)/boot.o $(BUILD)/kernel.o

$(BUILD)/iso: $(BUILD)/custom-os.bin grub/grub.cfg | $(BUILD)
	mkdir -p $(BUILD)/iso/boot/grub
	cp $(BUILD)/custom-os.bin $(BUILD)/iso/boot/custom-os.bin
	cp grub/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg
	touch $@

$(BUILD)/custom-os.iso: $(BUILD)/iso
	grub-mkrescue -o $@ $(BUILD)/iso

clean:
	rm -rf $(BUILD)

.PHONY: all clean
