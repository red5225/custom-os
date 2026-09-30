BUILD=build
CC=i686-linux-gnu-gcc
LD=i686-linux-gnu-ld
GRUB=grub-mkrescue

all: $(BUILD)/os.iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/multiboot.o: src/multiboot.asm | $(BUILD)
	nasm -f elf32 src/multiboot.asm -o $(BUILD)/multiboot.o

$(BUILD)/kernel.o: src/kernel.c | $(BUILD)
	$(CC) -m32 -march=i686 -ffreestanding -fno-pie -fno-stack-protector -fno-builtin -nostdlib -nodefaultlibs -nostartfiles -c src/kernel.c -o $(BUILD)/kernel.o

$(BUILD)/kernel.elf: $(BUILD)/multiboot.o $(BUILD)/kernel.o linker.ld
	$(LD) -m elf_i386 -T linker.ld -o $(BUILD)/kernel.elf $(BUILD)/multiboot.o $(BUILD)/kernel.o
	@test $$(stat -c%s $(BUILD)/kernel.elf) -le 65536 || (echo "Kernel ELF unexpectedly large" && exit 1)

$(BUILD)/isodir/boot/grub/grub.cfg: $(BUILD)/kernel.elf | $(BUILD)
	mkdir -p $(BUILD)/isodir/boot/grub
	cp $(BUILD)/kernel.elf $(BUILD)/isodir/boot/kernel.elf
	printf '%s\n' 'set timeout=0' 'set default=0' 'menuentry "Custom OS" {' '  multiboot /boot/kernel.elf' '  boot' '}' > $(BUILD)/isodir/boot/grub/grub.cfg

$(BUILD)/os.iso: $(BUILD)/isodir/boot/grub/grub.cfg
	$(GRUB) -o $(BUILD)/os.iso $(BUILD)/isodir

clean:
	rm -rf $(BUILD)
