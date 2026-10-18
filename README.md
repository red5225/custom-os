# CustomOS — Magneto Edition

CustomOS is a personal operating-system project for the Acer Chromebook CB314-3H family (x86-64, Intel Celeron N4500). It has its own identity, boot experience, text-mode interface, command tools, and **XC programming language**.

## XC — eXecutable Custom

XC is a small, readable language designed for CustomOS. Source files use the `.xc` extension. The interpreter is written in Python 3 for this first milestone; it is not yet a native compiler or CPU instruction set.

Run a program on CustomOS with:

```sh
xc apps/example/hello.xc
```

Or use the command dispatcher:

```sh
customos xc apps/example/hello.xc
```

Supported statements: `program`, `say`, `let`, `add`, `show`, `ask`, `if`, `else`, `end`, and `finish`. See `docs/XC-LANGUAGE.md` and `apps/example/hello.xc`. XC deliberately does not execute shell commands or arbitrary Python from source files.

## Current build architecture

This development image uses a Linux kernel and BusyBox userspace as a hardware-support foundation, with CustomOS-authored interface, XC language, and system branding on top. This is **not** a kernel written entirely from scratch. Upstream component licenses and attribution are documented in `THIRD_PARTY.md`.

The first UI milestone is a keyboard-driven full-screen interface on virtual terminal 1. This is a text-mode UI, not yet a graphical desktop with a mouse, compositor or hardware-accelerated graphics.

## Build

GitHub Actions builds the x86-64 ISO and uploads it as an artifact. Use **Actions → Build CustomOS Linux → Run workflow**. Download the `customos-linux-x86_64` artifact from the completed run. The workflow runs host-side tests for XC and other CustomOS tools before building.

## Test before installing

Use QEMU first. A successful QEMU boot does not prove Wi-Fi, audio, touchpad, suspend, internal storage or Chromebook firmware compatibility. The intended target is the Acer CB314-3H-C4V5, board family `magneto`, Intel Celeron N4500, x86-64.

## Credits

See `CREDITS.md` and `THIRD_PARTY.md`.
