# UEFI boot stage

`loader.c` is the native x86-64 UEFI application. It loads `kernel.elf`, collects the UEFI memory map and GOP framebuffer metadata, exits boot services, and transfers control to the freestanding kernel. `Makefile` builds the EFI application and GPT/FAT disk image. No Linux kernel or root filesystem is included.
