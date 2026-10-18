# CustomOS — Magneto Edition

CustomOS is a personal operating-system project for the Acer Chromebook CB314-3H family (x86-64, Intel Celeron N4500). It has its own name, boot experience, text-mode desktop interface, command tools and documentation.

## Current build architecture

This development image uses a Linux kernel and BusyBox userspace as a hardware-support foundation, with CustomOS-authored interface and system branding on top. This is **not** a kernel written entirely from scratch. Reusing upstream components is intentional for the current bootable development path; their licenses and attribution are documented in THIRD_PARTY.md.

The first UI milestone is a keyboard-driven full-screen interface on virtual terminal 1. The serial console remains available for boot diagnostics and QEMU testing. This is a text-mode UI, not yet a graphical desktop with a mouse, compositor or hardware-accelerated graphics.

## Build

GitHub Actions builds the x86-64 ISO and uploads it as an artifact. Push a commit or use Actions → Build CustomOS Linux → Run workflow. Download the customos-linux-x86_64 artifact from the completed run.

For a local build, see scripts/build.sh. The build requires the usual Buildroot host dependencies and network access to fetch pinned upstream sources.

## Test before installing

Use QEMU first. A successful QEMU boot proves only that the image boots in the tested virtual machine; it does not prove Wi-Fi, audio, touchpad, suspend, internal storage or Chromebook firmware compatibility.

The intended target is the Acer CB314-3H-C4V5, board family magneto, Intel Celeron N4500, x86-64. Confirm exact firmware state and device support before attempting physical boot. Firmware changes can make a Chromebook unbootable. Do not disable protections or modify firmware on a managed device.

## Credits

See CREDITS.md and THIRD_PARTY.md.
