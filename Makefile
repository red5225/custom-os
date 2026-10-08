CC=gcc
HOSTCC=gcc
CFLAGS=-m64 -march=x86-64 -mabi=sysv -ffreestanding -fno-stack-protector -fno-stack-check -fno-pie -fno-pic -mno-red-zone -mno-mmx -mno-sse -mcmodel=kernel -nostdinc -Ikernel -O2 -Wall -Wextra
LDFLAGS=-nostdlib -static -Wl,-z,max-page-size=0x1000 -Wl,-z,noexecstack -Wl,--build-id=none -Wl,-T,linker.ld
OBJ=kernel/kernel.o kernel/terminal.o kernel/keyboard.o kernel/xcl_runtime.o kernel/xcl_generated.o
XCL_SRCS=$(wildcard src/*.xcl)
XCLC=build/xclc
LIMINE_URL=https://github.com/Limine-Bootloader/Limine/releases/latest/download/limine-binary.tar.gz
LIMINE_DIR=limine-binary
LIMINE_BIN=$(LIMINE_DIR)/limine
LIMINE_H=kernel/limine.h
FONT_H=kernel/font8x8_basic.h

all: custom-os.elf

build/xclc: tools/xclc.c
	mkdir -p build
	$(HOSTCC) -std=c99 -O2 -Wall -Wextra $< -o $@

$(LIMINE_H):
	mkdir -p kernel
	curl -fsSL https://raw.githubusercontent.com/Limine-Bootloader/limine-protocol/trunk/include/limine.h -o $@

$(FONT_H):
	mkdir -p kernel
	curl -fsSL https://raw.githubusercontent.com/dhepper/font8x8/master/font8x8_basic.h -o $@

kernel/xcl_generated.c: $(XCL_SRCS) $(XCLC)
	$(XCLC) $(XCL_SRCS) $@

kernel/xcl_generated.o: kernel/xcl_generated.c $(LIMINE_H) $(FONT_H)
	$(CC) $(CFLAGS) -c $< -o $@

kernel/kernel.o: kernel/kernel.c $(LIMINE_H)
	$(CC) $(CFLAGS) -c $< -o $@

kernel/terminal.o: kernel/terminal.c $(FONT_H)
	$(CC) $(CFLAGS) -c $< -o $@

kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

custom-os.elf: $(OBJ) linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

$(LIMINE_BIN):
	curl -fL -o limine-binary.tar.gz $(LIMINE_URL)
	rm -rf $(LIMINE_DIR)
	gunzip < limine-binary.tar.gz | tar -xf -
	$(MAKE) -C $(LIMINE_DIR) CC="$(HOSTCC)"

iso: custom-os.elf $(LIMINE_BIN) limine.conf
	rm -rf iso-root
	mkdir -p iso-root/boot/limine iso-root/EFI/BOOT
	cp custom-os.elf iso-root/boot/custom-os.elf
	cp limine.conf $(LIMINE_DIR)/limine-uefi-cd.bin $(LIMINE_DIR)/limine-bios-cd.bin $(LIMINE_DIR)/limine-bios.sys iso-root/boot/limine/
	cp $(LIMINE_DIR)/BOOTX64.EFI iso-root/EFI/BOOT/BOOTX64.EFI
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso-root -o custom-os.iso
	$(LIMINE_BIN) bios-install custom-os.iso

hdd: custom-os.elf $(LIMINE_BIN) limine.conf
	rm -f custom-os.hdd
	dd if=/dev/zero bs=1M count=0 seek=64 of=custom-os.hdd
	sgdisk -Z custom-os.hdd
	sgdisk -n 1:2048:0 -t 1:ef00 -m 1 custom-os.hdd
	$(LIMINE_BIN) bios-install custom-os.hdd
	mformat -i custom-os.hdd@@1048576 -F ::
	mmd -i custom-os.hdd@@1048576 ::/EFI ::/EFI/BOOT ::/boot ::/boot/limine
	mcopy -i custom-os.hdd@@1048576 custom-os.elf ::/boot/custom-os.elf
	mcopy -i custom-os.hdd@@1048576 limine.conf ::/boot/limine/limine.conf
	mcopy -i custom-os.hdd@@1048576 $(LIMINE_DIR)/limine-bios.sys ::/boot/limine/limine-bios.sys
	mcopy -i custom-os.hdd@@1048576 $(LIMINE_DIR)/BOOTX64.EFI ::/EFI/BOOT/BOOTX64.EFI

clean:
	rm -rf build iso-root limine-binary limine-binary.tar.gz custom-os.elf custom-os.iso custom-os.hdd kernel/xcl_generated.c kernel/limine.h kernel/font8x8_basic.h
