# CustomOS

CustomOS is a small, terminal-first operating system built from its own kernel, terminal, shell, and XCL source code.

## Boot targets

The project now produces two boot artifacts:

- `custom-os.iso` — BIOS/Multiboot2 image for UTM/QEMU and legacy firmware.
- `custom-os-uefi.img` — GPT/FAT32 removable-disk image with `EFI/BOOT/BOOTX64.EFI` for modern UEFI machines such as compatible Chromebooks.

The UEFI image uses GRUB's x86_64 EFI loader and then loads the same CustomOS Multiboot2 kernel. The kernel itself remains custom and is not Linux.

## XCL

Most OS-facing code is authored in **XCL — eXecutable Core Language**.

The current XCL compiler is a small C bootstrap compiler. C and assembly are kept at the lowest hardware/boot boundary while XCL becomes the main language.

## Roadmap

1. Expand XCL variables, types, control flow, memory operations, and modules.
2. Replace the C bootstrap compiler with a native XCL compiler.
3. Add memory management and interrupts.
4. Add processes, syscalls, and a filesystem.
5. Add USB, networking, and selected hardware drivers.
6. Add x86_64 kernel execution while preserving the XCL-first architecture.
