# Custom OS

A small 32-bit x86 hobby OS designed for UTM/UTM SE.

## Interface

- Minimal desktop with a mountain/lake wallpaper
- One application: Terminal
- Keyboard input with Shift and Caps Lock
- PS/2 mouse support using polling for emulator compatibility
- Clean framebuffer rendering
- Text-mode fallback if a framebuffer is unavailable

## Terminal commands

help, clear, echo <text>, time, reboot

The wallpaper is an SVG rasterized during the GitHub Actions build, so the repository stays small while the ISO contains the image.

Low-level input/kernel components adapted from HelixaOS are documented in THIRD_PARTY.md.
