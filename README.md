# Custom OS — minimal x86 kernel ISO

A clean starting point for our own OS.

The ISO contains one tiny 32-bit x86 kernel loaded by GRUB BIOS. It writes HI FROM CUSTOM OS directly to VGA text memory.

Build: make
Output: build/custom-os.iso

UTM: Emulate -> Other -> Intel i440FX / x86, Legacy BIOS, UEFI off, attach the ISO.

Add features in kernel/kernel.c, boot/boot.S, linker.ld, iso/boot/grub/grub.cfg, and Makefile.

No Linux, Buildroot, Tiny Core, Python, desktop packages, or external root filesystem.
