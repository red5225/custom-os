# Source layout

- `boot/uefi/`: UEFI loader and boot-image Makefile.
- `kernel/`: freestanding kernel and linker script.
- `language/xc/`: XC language grammar and specification.
- `stdlib/xc/`: source modules written in XC.
- `apps/`: example XC programs.
- `tests/xc/`: executable XC regression programs.
- `tools/xc/`: host-side XC interpreter.
- `tools/host/`: C++ inspection and C# manifest validation utilities.

The XC interpreter currently runs on the host; it is not linked into the kernel.
