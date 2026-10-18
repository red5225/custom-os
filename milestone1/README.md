# Milestone 1 — Native UEFI boot and freestanding kernel

This isolated milestone adds a small x86-64 UEFI application written for GNU-EFI and a separately linked freestanding ELF kernel. It does not use a Linux kernel or bundle a Linux distribution. The existing project remains unchanged outside this directory and its dedicated workflow.

## Build locally (Ubuntu/Debian)

```sh
sudo apt-get update
sudo apt-get install -y build-essential binutils gcc gnu-efi qemu-system-x86 ovmf dosfstools mtools
make -C milestone1
```

The build creates `milestone1/build/esp/EFI/BOOT/BOOTX64.EFI`, `milestone1/build/esp/kernel.elf`, and a FAT EFI disk image at `milestone1/build/milestone1.img`.

## QEMU smoke test

```sh
make -C milestone1 test
```

The test uses OVMF and expects the serial output `NOL OS MILESTONE 1: kernel reached`. A successful QEMU test does not establish that the Chromebook firmware will boot the image.

## Hardware notes

Target: Acer Chromebook CB314-3H-C4V5, Intel Celeron N4500 (x86-64), board family reported as magneto. This is a generic x86-64 UEFI test image; Chromebook-specific firmware compatibility and device drivers are not implemented in this milestone.

The loader expects a simple ELF64 x86-64 kernel with PT_LOAD segments linked to their physical addresses. It allocates a small stack, passes a minimal boot-info structure in RDI, exits UEFI boot services, and jumps to the ELF entry point. Kernel diagnostics use COM1 serial so QEMU testing does not depend on a graphical framebuffer.
