# CustomOS

CustomOS is a small terminal-first operating system with its own kernel foundation and its own native programming language: NOL (Native OS Language).

## Design
- Custom freestanding kernel
- GRUB Multiboot2 boot path
- VGA terminal
- PS/2 keyboard support for the current development target
- No Linux kernel
- No desktop/UI dependency
- NOL is the native OS-facing language
- Most shell and OS-facing behavior is authored in NOL and compiled during the build
- C/assembly are kept for the lowest-level hardware/boot boundary

## NOL
NOL currently supports functions, command matching, output, terminal control, and calls. The bootstrap compiler is intentionally small. The long-term goal is NOL source -> NOL IR -> native machine code, removing the C translation layer.

## Roadmap
1. NOL lexer/parser and native backend
2. memory manager
3. interrupts and timer
4. processes and syscalls
5. filesystem
6. executable loader
7. USB
8. networking
9. optional x86_64/UEFI
10. NOL standard library and package system

Python is not part of the base OS. If supported later, it will be an optional userspace port.
