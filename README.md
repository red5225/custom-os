# CustomOS

CustomOS is a small custom freestanding x86 operating system. It is not a Linux distribution and does not use the Linux kernel.

## Current build
- Custom 32-bit kernel
- GRUB Multiboot2 boot
- VGA terminal fallback
- PS/2 keyboard
- PS/2 mouse cursor support when firmware exposes PS/2
- Minimal framebuffer desktop when GRUB provides a 32-bit framebuffer
- Automatic terminal fallback
- Linux-derived hardware work kept separate and credited
- GitHub Actions now validates the ISO and performs a QEMU boot smoke test

## Python
Python is NOT pre-installed yet. A real Python environment requires a userspace, filesystem, process model, syscalls, ELF loading, and a Python runtime port. It is planned after those kernel and userspace layers are stable.

## Hardware
The current ISO is mainly a UTM/legacy-x86 development build. Modern Chromebooks generally need x86_64 + UEFI + USB host/HID support. That is the next hardware milestone; the current 32-bit PS/2 build should not be treated as guaranteed Chromebook support.

## Build
GitHub Actions builds custom-os.iso and uploads it as the custom-os artifact.
