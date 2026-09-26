# custom-os

A small low-level x86 operating system designed to build into a bootable image for UTM.

## Goal

Build a tiny 32-bit x86 kernel and boot it in UTM using a bootable ISO image.

## Requirements

- NASM
- GCC with 32-bit support
- GNU binutils
- GRUB tooling (`grub-mkrescue`)
- xorriso
- UTM

## Build

Run:

```sh
make
```

The resulting `build/custom-os.iso` can be attached to a UTM virtual machine.

## Project layout

- `boot/` — Multiboot-compatible boot entry
- `kernel/` — C kernel code
- `linker.ld` — kernel linker script
- `Makefile` — build and ISO creation
- `grub/` — GRUB configuration
- `build/` — generated files (ignored by Git)

This is an educational OS project and intentionally starts small.