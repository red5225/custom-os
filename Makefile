BUILD=build
CC=i686-linux-gnu-gcc
LD=i686-linux-gnu-ld
OBJCOPY=i686-linux-gnu-objcopy

all: $(BUILD)/os.iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.bin: src/boot.asm | $(BUILD)
	nasm -f bin src/boot.asm -o $(BUILD)/boot.bin

$(BUILD)/kernel.bin: src/kernel.c | $(BUILD)
	$(CC) -m32 -march=i686 -ffreestanding -fno-pie -fno-stack-protector -fno-builtin -nostdlib -nostartfiles -nodefaultlibs -c src/kernel.c -o $(BUILD)/kernel.o
	$(LD) -m elf_i386 -Ttext 0x10000 --oformat elf32-i386 -o $(BUILD)/kernel.elf $(BUILD)/kernel.o
	$(OBJCOPY) -O binary $(BUILD)/kernel.elf $(BUILD)/kernel.bin
	@test $$(stat -c%s $(BUILD)/kernel.bin) -le 8192 || (echo "Kernel is larger than 8192 bytes; bootloader reads only 16 sectors" && exit 1)

$(BUILD)/os.iso: $(BUILD)/boot.bin $(BUILD)/kernel.bin | $(BUILD)
	cat $(BUILD)/boot.bin $(BUILD)/kernel.bin > $(BUILD)/os.img
	truncate -s 1474560 $(BUILD)/os.img
	genisoimage -quiet -o $(BUILD)/os.iso -b os.img -c boot.cat -no-emul-boot -boot-load-size 2880 -boot-info-table $(BUILD)/os.img

clean:
	rm -rf $(BUILD)
