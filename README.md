# CustomOS

CustomOS is a small custom freestanding x86 operating system. It is not a Linux distribution and does not use the Linux kernel.

## Current build
- Custom 32-bit kernel
- GRUB Multiboot2 boot
- VGA terminal fallback
- PS/2 keyboard
- PS/2 mouse cursor support when firmware exposes the PS/2 controller
- Minimal framebuffer desktop when GRUB provides a 32-bit framebuffer
- Automatic terminal fallback
- Linux-derived hardware work kept separate and credited

## Hardware note
UTM can emulate the PS/2 devices used by the current build. Modern Chromebooks generally expose USB HID devices instead, so Chromebook support needs the next hardware layer: x86_64/UEFI + USB host controller + USB HID.

The current ISO is therefore a development build for UTM/legacy x86 testing, not yet a guaranteed Chromebook image.
