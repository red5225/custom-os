# UEFI boot stage

`loader.c` is the native x86-64 UEFI application. It loads `kernel.elf`, collects the UEFI memory map and GOP framebuffer metadata, exits boot services, and transfers control to the freestanding kernel. `Makefile` builds the EFI application and GPT/FAT disk image. No Linux kernel or root filesystem is included.


XCUI is vendored under `ui/xcui`. The build validates the desktop description on the host, compiles it into static C data, and links the freestanding XCUI layout/render runtime into `kernel.elf`. The kernel uses its GOP framebuffer callbacks to draw the embedded desktop. Buttons are hit-testable by the XCUI runtime, but a live keyboard/mouse event loop and XC action registry are not wired yet. QEMU smoke tests confirm the kernel handoff only; they do not prove this image boots on a physical Chromebook.
