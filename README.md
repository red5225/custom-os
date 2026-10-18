# CustomOS

CustomOS is an experimental x86-64 operating-system project targeting UEFI PCs and, eventually, the Acer Chromebook CB314-3H family. The active boot path is the original UEFI application and freestanding kernel under `milestone1/`.

## No Linux distribution stack

The intended CustomOS output contains no Linux kernel, Linux root filesystem, Buildroot, GRUB Linux ISO, or Linux distribution. GitHub Actions uses an Ubuntu runner only as a build machine; that runner is not bundled into the generated UEFI artifacts.

## Build

Requirements: C compiler, binutils, GNU-EFI headers/libraries, `mtools`, `dosfstools`, QEMU x86, and OVMF firmware. On Ubuntu-based build hosts, install `build-essential binutils gcc gnu-efi mtools dosfstools qemu-system-x86 ovmf`.

```sh
make clean all
make test
```

These commands delegate to `milestone1/Makefile`. Check the CI log for actual artifact names and test results. Do not flash a raw disk image until its partition layout and boot mode have been verified.

## Active layout

- `milestone1/loader.c` — UEFI application/boot loader.
- `milestone1/kernel.c` — freestanding x86-64 kernel experiment.
- `milestone1/kernel.ld` — linker layout required by the kernel image; it is not disposable.
- `milestone1/Makefile` — native UEFI build, disk image creation, and QEMU smoke test.
- `src/*.xcl` and `tools/xclc.c` — early XCL language/bootstrap sources; not yet the kernel's primary implementation language.
- `tools/host/` — host-side C++ and C# diagnostic utilities.

## Status and limits

This is an early boot milestone, not a complete daily-use OS. QEMU success does not prove real Chromebook boot compatibility or Wi-Fi, audio, touchpad, power management, internal storage, or graphics support. Hardware support must be tested on the target device before being claimed.

The requested 500+ tracked-file goal is not met yet. Grow toward it with real memory management, interrupts, storage/filesystem code, drivers, XC compiler/runtime modules, and tests—not empty or duplicated files. CI reports the actual count rather than guessing.
