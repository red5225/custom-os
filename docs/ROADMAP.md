# Roadmap

1. Keep the UEFI loader and freestanding kernel build reproducible.
2. Test the loader and kernel under QEMU/OVMF; separately validate on target hardware before claiming compatibility.
3. Harden XC parsing, nested conditional handling, type-aware equality, diagnostics, and regression coverage.
4. Define a stable boot-info ABI shared by the loader and kernel.
5. Add independently tested memory-map management, interrupt descriptors, exception reporting, serial console, and framebuffer console modules.
6. Integrate a deliberately limited XC runtime only after its memory, I/O, and error model are specified.
7. Add storage/filesystem and device drivers only with hardware documentation and tests.

No step should add empty files merely to inflate the repository count.
