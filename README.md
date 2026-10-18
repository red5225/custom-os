# CustomOS

CustomOS is an experimental x86-64 operating system built around an original UEFI loader and a freestanding kernel. The OS image does not contain a Linux kernel, Linux root filesystem, Linux distribution, Buildroot, or GRUB/Linux boot chain. GitHub Actions uses Ubuntu only as a build host; that host is not bundled into the OS image.

## Build and test

Install a C/C++ toolchain, GNU-EFI, binutils, `mtools`, `dosfstools`, `fdisk`, QEMU and OVMF. Then run:

```sh
make clean all
make test
make xc-test
```

The native boot code lives in `boot/uefi/` and `kernel/`. The output is `boot/uefi/build/customos.img`; it is a raw GPT disk image containing a FAT EFI System Partition, not an ISO. Inspect its partition layout before flashing. The QEMU test is not proof of Chromebook hardware compatibility.

## XC

XC is the project’s custom command language. Its current host interpreter is `tools/xc/xc.cpp`; programs, grammar, specification, and standard-library source are under `language/xc/`, `stdlib/xc/`, `apps/`, and `tests/xc/`. The host interpreter is not yet embedded in the kernel, and XC is not yet the kernel implementation language.

## Honest status

This is an early bootable-kernel experiment, not a daily-use OS. Wi-Fi, audio, touchpad, storage drivers, power management, and Chromebook-specific firmware compatibility are not claimed. File-count targets should be met with real modules and tests, not empty files or duplicated filler.
