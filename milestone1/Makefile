SHELL := /bin/bash
ROOT := $(CURDIR)
BUILD := $(ROOT)/build
ESP := $(BUILD)/esp
EFI_INC := /usr/include/efi
EFI_ARCH_INC := /usr/include/efi/x86_64
EFI_LDS := /usr/lib/elf_x86_64_efi.lds
EFI_LIBDIR := /usr/lib
EFI_GNUEFI_LIBDIR := /usr/lib/gnuefi
EFI_CFLAGS := -I$(EFI_INC) -I$(EFI_ARCH_INC) -DEFI_FUNCTION_WRAPPER -fpic -fshort-wchar -mno-red-zone -fno-stack-protector -fno-stack-check -fno-asynchronous-unwind-tables -fno-unwind-tables -O2

.PHONY: all clean test
all: $(ESP)/EFI/BOOT/BOOTX64.EFI $(ESP)/kernel.elf $(BUILD)/milestone1.img

$(BUILD):
	mkdir -p $@

$(BUILD)/loader.o: $(ROOT)/loader.c | $(BUILD)
	$(CC) $(EFI_CFLAGS) -c $< -o $@

$(BUILD)/loader.so: $(BUILD)/loader.o
	$(LD) -nostdlib -znocombreloc -T $(EFI_LDS) -shared -Bsymbolic -L$(EFI_LIBDIR) -L$(EFI_GNUEFI_LIBDIR) $< -lefi -lgnuefi -o $@

$(BUILD)/BOOTX64.EFI: $(BUILD)/loader.so
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64 $< $@

$(ESP)/EFI/BOOT/BOOTX64.EFI: $(BUILD)/BOOTX64.EFI
	mkdir -p $(dir $@)
	cp $< $@

$(BUILD)/kernel.o: $(ROOT)/kernel.c | $(BUILD)
	$(CC) -std=c11 -Wall -Wextra -Werror -ffreestanding -fno-stack-protector -fno-pic -fno-pie -fno-asynchronous-unwind-tables -fno-unwind-tables -mno-red-zone -mcmodel=small -O2 -c $< -o $@

$(BUILD)/kernel.elf: $(BUILD)/kernel.o $(ROOT)/kernel.ld
	$(LD) -nostdlib -static -z max-page-size=0x1000 -T $(ROOT)/kernel.ld -o $@ $(BUILD)/kernel.o
	readelf -h -l $@

$(ESP)/kernel.elf: $(BUILD)/kernel.elf
	mkdir -p $(ESP)
	cp $< $@

$(BUILD)/milestone1.img: $(BUILD)/BOOTX64.EFI $(ESP)/EFI/BOOT/BOOTX64.EFI $(ESP)/kernel.elf
	dd if=/dev/zero of=$@ bs=1M count=16 status=none
	mkfs.fat -F 16 $@
	mmd -i $@ ::/EFI ::/EFI/BOOT
	mcopy -i $@ $(BUILD)/BOOTX64.EFI ::/EFI/BOOT/BOOTX64.EFI
	mcopy -i $@ $(BUILD)/kernel.elf ::/kernel.elf

clean:
	rm -rf $(BUILD)

test: all
	@set -eu; \
	CODE=/usr/share/OVMF/OVMF_CODE.fd; VARS_TEMPLATE=/usr/share/OVMF/OVMF_VARS.fd; \
	if [[ ! -f "$$CODE" || ! -f "$$VARS_TEMPLATE" ]]; then echo 'OVMF firmware files not found'; exit 2; fi; \
	cp "$$VARS_TEMPLATE" $(BUILD)/OVMF_VARS.fd; \
	rm -f $(BUILD)/serial.log; \
	(timeout 15s qemu-system-x86_64 -machine q35 -m 256M -cpu qemu64 -drive if=pflash,format=raw,readonly=on,file="$$CODE" -drive if=pflash,format=raw,file=$(BUILD)/OVMF_VARS.fd -drive format=raw,file=$(BUILD)/milestone1.img -serial file:$(BUILD)/serial.log -display none -monitor none -no-reboot) >/dev/null 2>&1 || true; \
	cat $(BUILD)/serial.log 2>/dev/null || true; \
	grep -q 'NOL OS MILESTONE 1: kernel reached' $(BUILD)/serial.log || { echo 'QEMU smoke test failed: expected kernel serial marker not found'; exit 1; }; \
	echo 'QEMU smoke test passed.'
