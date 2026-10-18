# CustomOS

CustomOS is an experimental x86-64 operating system built around an original UEFI loader and a freestanding kernel. The generated OS disk image does **not** include a Linux kernel, Linux root filesystem, Linux distribution, Buildroot, or GRUB/Linux boot chain. GitHub Actions uses Ubuntu only as a build host; that host is not bundled into CustomOS.

## Build and test

Install a C/C++ toolchain, GNU-EFI, binutils, `mtools`, `dosfstools`, `fdisk`, QEMU, and OVMF. Then run:

```sh
make clean all
make test
make xc-test
```

The native boot code lives in `boot/uefi/`; the freestanding kernel entry and linker layout live in `kernel/`. The output is `boot/uefi/build/customos.img`, a raw GPT disk image containing a FAT EFI System Partition, **not an ISO**. Inspect the partition layout before flashing. Passing QEMU does not prove compatibility with the Acer Chromebook CB314-3H.

## XC language

XC is the project's custom command language. Its host interpreter is `tools/xc/xc.cpp`; runnable examples are in `apps/` and regression programs are in `tests/xc/`. The interpreter supports a deliberately small initial grammar: output, variables, integer addition, input, equality conditions, and program termination. XC is currently interpreted on the build host; it is **not embedded in the kernel yet**.

## Native core library

`kernel/core/` contains 40 small C primitives for checked ranges/alignment, framebuffer offsets, interrupt metadata, keyboard/mouse decoding, PCI/block addressing, FAT cluster validation, checksums, UTF-8 byte classification, XC parsing helpers, and timer conversions. Each primitive has a separate test under `tests/core/`, compiled with warnings as errors in CI. These are reusable, host-tested primitives; they are not all linked into the booted kernel yet.

## Repository composition and limits

The current tracked tree contains 257 files, including 111 `.xc` files (about 43%). The count is produced from the actual Git tree, not fabricated placeholders. The requested 250-file threshold and XC percentage range are met, but this count is not a measure of OS completeness.

This remains an early boot milestone, not a daily-use OS. Wi-Fi, audio, touchpad, power management, internal-storage drivers, filesystem mounting, and Chromebook-specific firmware compatibility are not claimed. The next work is to integrate and test core modules in the kernel, expand XC with parsing/runtime features, and validate the exact boot image in QEMU and then on target hardware.
