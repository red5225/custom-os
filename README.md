# CustomOS — Magneto Edition

CustomOS is a personal operating-system project targeting the Acer Chromebook CB314-3H family (x86-64, Intel Celeron N4500). The repository's `main` branch is the integration line: it contains the current boot/build path and the pieces selected to ship together.

## Branch model

- `main` — integrated boot path, build workflow, tests, and release candidate. Changes should land here only after the relevant subsystem work is integrated.
- `system/boot` — boot configuration, UEFI/bootloader entry points, image layout, and boot smoke tests.
- `system/kernel` — low-level kernel code, hardware-facing routines, and kernel tests.
- `system/terminal` — terminal, keyboard input, shell commands, and text-mode interaction.
- `system/language-xc` — XC language syntax, interpreter/runtime, examples, and tests.
- `system/ui` — first-boot and text UI behavior.
- `system/apps` — app manifests, sample apps, and app launch/validation tools.
- `system/build-ci` — build scripts, packaging, CI, and emulator smoke tests.

Each subsystem branch is a full Git snapshot, not a separate repository or a folder by itself. Keep changes on a subsystem branch focused on that area, then merge the branch into `main` after its tests pass. See [`docs/BRANCH-MAP.md`](docs/BRANCH-MAP.md).

## Current implementation status

The current general-purpose ISO build still uses a Linux kernel and Buildroot/BusyBox userspace with CustomOS-authored tools and branding. It is **not** a kernel written entirely from scratch. The separate `milestone1` code is an experimental native UEFI loader/kernel path and is not proof of full Chromebook hardware support. XC currently has an initial interpreter and is not yet a native compiler.

## Build and test

GitHub Actions → **Build CustomOS Linux** builds the x86-64 ISO and runs host-side tests plus a QEMU serial-console smoke test. Download the `customos-linux-x86_64` artifact from a completed run. A successful QEMU smoke test does not prove that Wi-Fi, audio, touchpad, suspend, internal storage, or Chromebook firmware support works on physical hardware.

Run host-side tests locally with:

```sh
python3 -m unittest discover -s tests -v
```

For build details and compatibility limits, see [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) and [`docs/CHROMEBOOK-COMPATIBILITY.md`](docs/CHROMEBOOK-COMPATIBILITY.md).
