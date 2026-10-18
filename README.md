# CustomOS

CustomOS is an experimental x86-64 operating system built around an original UEFI loader and a freestanding kernel. The generated OS disk image does **not** include a Linux kernel, Linux root filesystem, Linux distribution, Buildroot, or GRUB/Linux boot chain. GitHub Actions uses Ubuntu only as a build host; that host is not bundled into CustomOS.

## Build and test

Install a C/C++ toolchain, GNU-EFI, binutils, `mtools`, `dosfstools`, `fdisk`, QEMU, and OVMF. Then run:

```sh
make clean all
make test
make xc-test
```

The native boot code lives in `boot/uefi/`; the freestanding kernel entry and linker layout live in `kernel/`. The outputs are `boot/uefi/build/customos.iso` (UEFI El Torito ISO) and `boot/uefi/build/customos.img` (raw GPT disk image containing a FAT EFI System Partition). CI checks that the ISO is under 200 MiB and boots both formats in QEMU/OVMF. Passing QEMU does not prove compatibility with the Acer Chromebook CB314-3H.

## XC language

XC is the project's custom command language. Its host interpreter is `tools/xc/xc.cpp`; runnable examples are in `apps/` and regression programs are in `tests/xc/`. The interpreter supports a deliberately small initial grammar: output, variables, integer addition, input, equality conditions, and program termination. XC is currently interpreted on the build host; it is **not embedded in the kernel yet**.

## Native core library

`kernel/core/` contains 40 small C primitives for checked ranges/alignment, framebuffer offsets, interrupt metadata, keyboard/mouse decoding, PCI/block addressing, FAT cluster validation, checksums, UTF-8 byte classification, XC parsing helpers, and timer conversions. Each primitive has a separate test under `tests/core/`, compiled with warnings as errors in CI. These are reusable, host-tested primitives; they are not all linked into the booted kernel yet.

## Repository composition and limits

The current tracked tree contains 257 files, including 111 `.xc` files (about 43%). The count is produced from the actual Git tree, not fabricated placeholders. The requested 250-file threshold and XC percentage range are met, but this count is not a measure of OS completeness.

This remains an early boot milestone, not a daily-use OS. Wi-Fi, audio, touchpad, power management, internal-storage drivers, filesystem mounting, and Chromebook-specific firmware compatibility are not claimed. The next work is to integrate and test core modules in the kernel, expand XC with parsing/runtime features, and validate the exact boot image in QEMU and then on target hardware.


## Booting on the Acer Chromebook CB314-3H (MAGNETO)

The current artifact contains an original x86-64 UEFI application and kernel; it is not a ChromeOS kernel image. It can boot only when the device firmware exposes a standard UEFI USB boot path (for example, compatible Full ROM UEFI firmware). Developer Mode alone does **not** turn the stock ChromeOS boot menu into a general UEFI boot menu.

The ISO now exposes its FAT EFI boot image as a GPT **EFI System Partition** rather than only labeling it as generic Basic Data. CI checks the partition table and runs the image through QEMU/OVMF. Those checks validate the image format and virtual UEFI path, not the Chromebook's current firmware state or hardware.

For a UEFI firmware boot test, use `customos.img` from the build artifact as the first troubleshooting choice: it is a raw GPT disk image with a FAT EFI System Partition and `EFI/BOOT/BOOTX64.EFI`. Write the **image itself** to a USB drive with an image-writing tool; this erases the selected USB drive. The ISO is also provided for optical-media/ISO workflows.

If the Chromebook shows the normal ChromeOS Developer Mode screen with Ctrl+D/Ctrl+U, or simply returns to that screen after choosing USB, stop repeating the same flash. That firmware path does not directly launch this generic UEFI loader. This build does not install or modify firmware, and it has not been confirmed booting on physical MAGNETO hardware.
