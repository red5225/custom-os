# Custom OS

Custom OS is now built on top of a tiny Linux system instead of a hand-written kernel.

The build uses Buildroot, which produces a small Linux kernel, BusyBox userspace, initramfs, and a BIOS-bootable ISO. Buildroot's current stable release is 2026.08.

## Target

- Intel i386/x86 PC
- UTM on Intel Mac
- Legacy BIOS
- UEFI disabled
- Small ISO
- Linux kernel + BusyBox base
- Custom boot screen and shell

## Build

GitHub Actions downloads Buildroot 2026.08, configures the x86 PC target, adds the Custom OS root filesystem overlay, builds the Linux kernel/userspace, and creates a bootable ISO.

The generated ISO is uploaded as the custom-os-linux-iso artifact.

## UTM

Choose Emulate -> Other -> Intel i440FX-based PC, use 512 MB RAM, keep UEFI disabled, and attach the generated ISO as the CD/DVD.

This Linux-based approach gives us a real kernel, device drivers, process management, filesystems, networking support, and BusyBox userspace while keeping the base small.
