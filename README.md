# CustomOS — Magneto Edition

CustomOS is a personal operating-system project targeting the Acer Chromebook CB314-3H family (x86-64, Intel Celeron N4500). The repository's `main` branch is the integration line: it contains the current boot/build path and the pieces selected to ship together.

## Branch model

- `main` — integrated boot path, component catalog, build workflow, tests, and release candidate. This is the branch used for the combined ISO build.
- `system/boot` — boot configuration, UEFI/bootloader entry points, image layout, and boot smoke tests.
- `system/kernel` — low-level kernel code, hardware-facing routines, and kernel tests.
- `system/terminal` — terminal, keyboard input, shell commands, and text-mode interaction.
- `system/language-xc` — XC language syntax, interpreter/runtime, examples, and tests.
- `system/ui` — first-boot and text UI behavior.
- `system/apps` — app manifests, sample apps, and app launch/validation tools.
- `system/build-ci` — build scripts, packaging, CI, and emulator smoke tests.

Subsystem branches are testing/work areas. Test changes there, then merge approved work into `main`; keep `main` as the integrated build branch. Each branch is a full Git snapshot, not a physically separate directory. See [`docs/BRANCH-MAP.md`](docs/BRANCH-MAP.md).

## Component catalog (500+ new files)

The `components/` tree contains 100 independently compiled component packages across boot, kernel, memory, process/task, storage, drivers, terminal, XC language, UI, and security. Each package contains five files: a C public header, C implementation, executable self-test, JSON manifest, and README. CI compiles and runs all 100 tests. These are intentionally labelled **catalog scaffolds**: they give each subsystem a stable interface and test boundary, but they are not yet wired into the boot image and do not claim to implement hardware drivers or a complete kernel. See [`docs/COMPONENT-CATALOG.md`](docs/COMPONENT-CATALOG.md).

## Current implementation status

The current general-purpose ISO build still uses a Linux kernel and Buildroot/BusyBox userspace with CustomOS-authored tools and branding. It is **not** a kernel written entirely from scratch. The separate `milestone1` code is an experimental native UEFI loader/kernel path and is not proof of full Chromebook hardware support. XC currently has an initial interpreter and is not yet a native compiler.

## Build and test

GitHub Actions → **Build CustomOS Linux** builds the x86-64 ISO, runs host-side tests, compiles the component catalog, and performs a QEMU serial-console smoke test. Download the `customos-linux-x86_64` artifact from a completed successful run. A successful QEMU smoke test does not prove that Wi-Fi, audio, touchpad, suspend, internal storage, or Chromebook firmware support works on physical hardware.

Run checks locally:

```sh
python3 -m unittest discover -s tests -v
python3 scripts/test_component_catalog.py
```

For build details and compatibility limits, see [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) and [`docs/CHROMEBOOK-COMPATIBILITY.md`](docs/CHROMEBOOK-COMPATIBILITY.md).
