# CustomOS

CustomOS is an experimental x86-64 UEFI operating system with an original kernel and a language-first platform. The project is being organized around three explicit layers:

- **`c-kernel/`** — C-only kernel core and lowest-level hardware primitives.
- **`xc-language/`, `xc-drivers/`, `xc-services/`** — XC language/compiler/runtime, device drivers, and system services.
- **`xcui-runtime/`** — XCUI parsing, UI declarations, rendering contracts, and event-to-XC dispatch.

The intended pipeline is **XCUI → XC → C/native code**. Higher-level drivers and services belong in XC; C is not the default implementation language for OS subsystems.

## What is implemented now

- UEFI boot path and a small freestanding kernel are present in the existing boot/kernel build.
- The XC host interpreter supports a deliberately small initial syntax.
- An initial XC-to-C code generator and CI smoke tests exist on the language-platform branch.
- The new `xc-language/`, `xc-drivers/`, `xc-services/`, and `xcui-runtime/` areas contain platform interfaces and implementation contracts, not claims of complete drivers or a finished compiler.

## Build and checks

Install a C/C++ toolchain, GNU-EFI, binutils, mtools, dosfstools, fdisk, QEMU, and OVMF. Run:

```sh
make clean all
make test
make xc-test
make xcui-test
```

GitHub Actions checks the native build and language smoke tests. A passing QEMU test does not prove compatibility with the physical Acer Chromebook CB314-3H.

## Naming rules

- Any C kernel/low-level hardware directory starts with `c-`.
- XC-only directories start with `xc-`.
- XCUI-only directories start with `xcui-`.
- Examples and tests stay in explicitly named `examples/` and `tests/` folders and are never presented as implementations.

See [`docs/XC_LANGUAGE_PLATFORM.md`](docs/XC_LANGUAGE_PLATFORM.md) and the layer contracts in the new top-level directories for current boundaries and limitations.