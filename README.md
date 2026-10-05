# CustomOS

A small custom x86 OS with its own kernel, terminal, boot flow, PS/2 mouse support, and a minimal framebuffer desktop.

## Current
- Custom freestanding 32-bit kernel
- Multiboot2 boot
- VGA terminal fallback
- PS/2 keyboard
- PS/2 mouse polling
- Minimal framebuffer UI with mouse cursor
- Linux-derived hardware work kept separate and credited

## Hardware note
The current kernel is 32-bit and the mouse driver targets PS/2. This is useful in UTM and older PC-style hardware, but a modern Chromebook normally needs x86_64 UEFI plus USB HID support. That is the next Chromebook hardware milestone.

## Build
GitHub Actions builds `custom-os.iso`.
