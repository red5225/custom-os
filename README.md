# CustomOS

Small custom x86 OS with its own kernel, terminal, boot flow, and minimal framebuffer desktop.

## Current
- Freestanding 32-bit custom kernel
- Multiboot2 boot
- VGA terminal fallback
- PS/2 keyboard
- PS/2 mouse polling
- Minimal framebuffer desktop and mouse cursor
- Explicit Linux/GRUB credits

## Python
Python is **not pre-installed yet**. A real Python + pip environment requires CustomOS userspace, syscalls, memory/process support, filesystem, an executable loader, and then a Python runtime port.

## Hardware roadmap
The current build is useful in UTM. Modern Chromebook support still needs x86_64/UEFI and USB HID support.
