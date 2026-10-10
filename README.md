# CustomOS

CustomOS is a Linux-based operating system project with its own identity, terminal tools, application manifest, integrity/recovery prototype, and planned native desktop/UI framework.

## Architecture
The previous hand-written kernel and bootloader experiments were unreliable on real hardware. The project now uses **Buildroot + the Linux kernel + BusyBox init/userspace + Python 3**. CustomOS identity lives in its own services, application ecosystem and planned desktop rather than a renamed wallpaper.

Buildroot builds the kernel, toolchain and root filesystem together. Manual: https://buildroot.org/downloads/manual/manual.html

## Build and test
GitHub Actions downloads pinned Buildroot 2026.08, builds the x86-64 kernel and initramfs, packages them into a GRUB ISO, runs host unit tests and boots the ISO in QEMU with a serial-console assertion.

Local prerequisites: GNU make, GCC, curl, Python 3, xorriso, GRUB tools and standard Buildroot dependencies. Run `sh ./scripts/build.sh` to build the kernel/root filesystem locally.

## First milestone
- Generic x86-64 Linux kernel.
- BusyBox init and terminal shell.
- Python 3 and pip in the root filesystem.
- customos help/about/status/python/integrity/apps commands.
- SHA-256 integrity-check/recovery prototype.
- Application manifest validator.
- QEMU boot smoke test.

## Not yet delivered
No graphical desktop, Python IDE, package installer, native UI markup renderer, physical Chromebook validation, or signed/tamper-resistant recovery is claimed complete. Those are subsequent milestones after the image passes boot tests.

## Chromebook safety
The first image is a generic x86-64 test target, not a guarantee for every Chromebook. The CB314-3H's exact SKU, firmware mode, management status and wireless chipset have not been confirmed here. Do not modify firmware or disable protections on a managed device; firmware changes can make a Chromebook unbootable.
