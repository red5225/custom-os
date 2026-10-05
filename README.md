# CustomOS

A small custom x86 OS with its own kernel, terminal, boot flow, and minimal framebuffer desktop.

## Current
- Custom freestanding 32-bit kernel
- Multiboot2 boot
- VGA terminal fallback
- PS/2 keyboard
- PS/2 mouse polling
- Minimal framebuffer UI with mouse cursor
- Linux-derived hardware work kept separate and credited

## Hardware note
The current kernel is 32-bit. It can boot in UTM, but a modern Chromebook normally needs a 64-bit UEFI boot path plus USB HID support. The next hardware milestone is x86_64 + UEFI + USB.

## Build
GitHub Actions builds `custom-os.iso`.
