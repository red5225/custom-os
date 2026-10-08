# CustomOS

CustomOS is a small terminal-first operating system with its own kernel core and **XCL (eXecutable Core Language)** as the main OS-facing source language.

## Boot architecture
CustomOS now uses **Limine** instead of the previous fragile GRUB boot path.

- `custom-os.iso` — hybrid BIOS/UEFI ISO for UTM/QEMU and compatible UEFI PCs.
- `custom-os.hdd` — raw removable-disk image with BIOS boot support and a FAT32 EFI partition.

The x86-64 kernel requests a framebuffer from Limine and renders its terminal directly into it, avoiding dependence on VGA text mode.

## XCL
XCL remains the main OS-facing source language. The current compiler is a small C bootstrap compiler that translates XCL to freestanding C; the native XCL compiler is the next language milestone.

No Python is required by the build.

## Chromebook
CB314-3H is the MAGNETO platform and current firmware documentation lists both RW_LEGACY and UEFI Full ROM options. Stock ChromeOS verified boot is not the same as generic PC UEFI, so the device must be configured for custom OS booting before a raw USB image can be accepted.

Do not flash over a USB drive containing data you need.
