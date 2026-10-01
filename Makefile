BUILD := build
CC := gcc
LD := ld
CFLAGS := -m32 -ffreestanding -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -O2 -Wall -Wextra
LDFLAGS := -m elf_i386 -T linker.ld
.PHONY: all clean
all: $(BUILD)/custom-os.iso
$(BUILD):
	mkdir -p $(BUILD)
$(BUILD)/boot.o: boot/boot.S | $(BUILD)
	$(CC) -m32 -c $< -o $@
$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD)/kernel.bin: $(BUILD)/boot.o $(BUILD)/kernel.o linker.ld
	$(LD) $(LDFLAGS) -o $@ $(BUILD)/boot.o $(BUILD)/kernel.o
	grub-file --is-x86-multiboot $@
$(BUILD)/custom-os.iso: $(BUILD)/kernel.bin iso/boot/grub/grub.cfg
	rm -rf $(BUILD)/iso
	mkdir -p $(BUILD)/iso/boot/grub
	cp $(BUILD)/kernel.bin $(BUILD)/iso/boot/kernel.bin
	cp iso/boot/grub/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(BUILD)/iso >/dev/null
	isoinfo -d -i $@ >/dev/null
clean:
	rm -rf $(BUILD)
