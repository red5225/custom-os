BUILD=build

all: $(BUILD)/os.iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.bin: src/boot.asm | $(BUILD)
	nasm -f bin src/boot.asm -o $(BUILD)/boot.bin

$(BUILD)/kernel.bin: src/kernel.c | $(BUILD)
	i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -fno-builtin -nostdlib -nostartfiles -nodefaultlibs -c src/kernel.c -o $(BUILD)/kernel.o
	i686-elf-ld -m elf_i386 -Ttext 0x10000 -o $(BUILD)/kernel.elf $(BUILD)/kernel.o
	i686-elf-objcopy -O binary $(BUILD)/kernel.elf $(BUILD)/kernel.bin

$(BUILD)/os.iso: $(BUILD)/boot.bin $(BUILD)/kernel.bin | $(BUILD)
	cat $(BUILD)/boot.bin $(BUILD)/kernel.bin > $(BUILD)/os.img
	truncate -s 1474560 $(BUILD)/os.img
	genisoimage -quiet -o $(BUILD)/os.iso -b os.img -no-emul-boot $(BUILD)/os.img

clean:
	rm -rf $(BUILD)
