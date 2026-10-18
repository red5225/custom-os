# CustomOS branch map

## Purpose

`main` is the integration branch and source of the buildable release candidate. It should contain the selected boot path, subsystem code, automated tests, and build workflow together. Subsystem branches are work areas; they do not physically contain only one directory because Git branches represent complete repository snapshots.

## Ownership map

| Branch | Primary ownership | Typical paths |
|---|---|---|
| `system/boot` | Boot entry, boot configuration, disk/ISO layout, boot smoke tests | `milestone1/`, `iso/`, `limine.conf`, boot-related workflow steps |
| `system/kernel` | Kernel core and hardware-facing routines | `kernel/`, `linker.ld` |
| `system/terminal` | Terminal, keyboard, shell and command dispatch | `kernel/terminal.*`, `kernel/keyboard.*`, `src/shell.*`, `src/terminal.*`, `src/help.*` |
| `system/language-xc` | XC language definition, runtime/interpreter, compiler experiments, examples and tests | `XCL.md`, `docs/XC-LANGUAGE.md`, `kernel/xcl_runtime.*`, `tools/xclc.c`, `src/*.xcl`, `tests/test_xc.py` |
| `system/ui` | First-boot and text UI | `buildroot-external/board/customos/overlay/usr/bin/customos-ui`, `docs/UI-MILESTONE.md`, UI assets |
| `system/apps` | App manifest format, example apps, validation and launch tools | `apps/`, `buildroot-external/board/customos/overlay/usr/bin/customos-app` |
| `system/build-ci` | Reproducible build, packaging, CI, QEMU checks and artifacts | `Makefile`, `scripts/`, `.github/workflows/`, `tests/` |

## Contribution flow

1. Start from the latest `main` commit when creating or refreshing subsystem work.
2. Keep a commit scoped to the subsystem branch's ownership area. If a change affects more than one subsystem, document the cross-component dependency.
3. Run the relevant unit tests and build/smoke tests before integration.
4. Merge the tested subsystem branch into `main`; `main` is the only branch treated as the integrated build candidate.
5. Trigger or inspect the `Build CustomOS Linux` workflow on `main` and check the actual conclusion and artifact before calling a build successful.

## Important architecture caveat

The current ISO workflow builds a Linux kernel plus Buildroot/BusyBox userspace. This branch map organizes the current project; it does not transform that implementation into a fully original kernel. The native UEFI milestone remains experimental until its boot path is integrated and verified independently.
