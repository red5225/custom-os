# UEFI boot stage

`loader.c` is the native x86-64 UEFI application. It loads `kernel.elf` from the root of the EFI System Partition, collects the UEFI memory map and GOP framebuffer metadata, exits boot services, and transfers control to the freestanding kernel. `Makefile` builds the EFI application and GPT/FAT disk image. No Linux kernel or root filesystem is included.

## Chromebook boot path

The screen listing `local image A`, `local image B`, `verified image A`, `verified image B`, and `Alternate USB Boot` is the Chromebook's legacy/AltFw GRUB menu, not the CustomOS menu. Its `Alternate USB Boot` entry attempts to load a Linux-style `/boot/vmlinuz`, so it cannot launch this project's `kernel.elf` and will report `boot vmlinuz not found`. Renaming `kernel.elf` to `vmlinuz` is not a valid fix because this build uses a native UEFI application and an ELF kernel handoff.

Use the firmware's UEFI/EDK II boot manager instead:

1. Power on and watch for the coreboot/firmware splash; press `Esc` when prompted to enter the firmware menu. Do not choose `Alternate USB Boot` in the GRUB menu.
2. Open **Boot Manager** and select the USB device if it appears.
3. If there is **Boot From File**, open the USB's EFI System Partition and select `EFI/BOOT/BOOTX64.EFI`.
4. The expected first message from our loader is `CustomOS: native UEFI loader`; after a successful handoff the kernel reports `CUSTOMOS: kernel reached` on serial in QEMU.

If the firmware menu does not offer Boot Manager/Boot From File, this machine may be entering the Chromebook's RW_LEGACY AltFw path rather than full UEFI firmware. The current image is built for x86-64 UEFI; QEMU tests do not prove physical-device compatibility. Do not flash internal firmware as part of this test. The `.img` file is a raw disk image intended to be written to a USB drive; writing it erases that USB drive.

## XCUI status

XCUI is vendored under `ui/xcui`. The build validates the desktop description on the host, compiles it into static C data, and links the freestanding XCUI layout/render runtime into `kernel.elf`. The kernel uses its GOP framebuffer callbacks to draw the embedded desktop. Buttons are hit-testable by the XCUI runtime, but a live keyboard/mouse event loop and XC action registry are not wired yet. QEMU smoke tests confirm the kernel handoff only; they do not prove this image boots on a physical Chromebook.
