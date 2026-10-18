# CustomOS

CustomOS is an experimental x86-64 operating-system project targeting UEFI PCs and, eventually, the Acer Chromebook CB314-3H family. The active boot path is the original UEFI application and freestanding kernel under `milestone1/`.

## No Linux distribution stack

The intended CustomOS output contains no Linux kernel, Linux root filesystem, Buildroot, GRUB Linux ISO, or Linux distribution. GitHub Actions uses an Ubuntu runner only as a build machine; that runner is not bundled into the generated UEFI artifacts.

## Build

Requirements: C compiler, C++ compiler, binutils, GNU-EFI headers/libraries, `mtools`, `dosfstools`, QEMU x86, and OVMF firmware. On Ubuntu-based build hosts, install `build-essential binutils gcc g++ gnu-efi mtools dosfstools qemu-system-x86 ovmf`.

```sh
make clean all
make test
```

These commands delegate to `milestone1/Makefile`. Check CI for actual artifact names and test results. Do not flash a raw disk image until its partition layout and boot mode have been verified.

## Active layout

- `milestone1/loader.c` — UEFI application/boot loader.
- `milestone1/kernel.c` — freestanding x86-64 kernel experiment.
- `milestone1/kernel.ld` — linker layout required by the kernel image; it is necessary, not dead code.
- `milestone1/Makefile` — native UEFI build, disk image creation, and QEMU smoke test.
- `tools/xc/xc.cpp` — host-side interpreter for the custom `.xc` language.
- `apps/example/hello.xc` and `tests/xc/` — executable language example and smoke-test programs.
- `tools/host/os_inspect.cpp` — C++ host diagnostic utility.
- `tools/host/ManifestCheck/` — C# manifest validator.

The XC interpreter currently runs on the build host. It is not yet embedded in the freestanding kernel; that integration is planned, not claimed as complete.

## Status and limits

This is an early boot milestone, not a complete daily-use OS. QEMU success does not prove real Chromebook boot compatibility or Wi-Fi, audio, touchpad, power management, internal storage, or graphics support. Hardware support must be tested on the target device before being claimed.

The requested 500+ tracked-file goal is not met yet. Grow toward it with real memory management, interrupts, storage/filesystem code, drivers, XC runtime/library modules, and tests—not empty or duplicated files. CI reports the actual count rather than guessing.
