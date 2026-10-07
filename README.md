# CustomOS

CustomOS is a small terminal-first operating system with its own kernel foundation and its own programming language: NOL (Native OS Language).

## Core idea

NOL is the main language.

Most OS-facing behavior is stored in .nol files under src/. The current C and assembly code is deliberately kept small and low-level:

- boot entry
- VGA terminal primitives
- PS/2 keyboard input
- tiny NOL bootstrap runtime

There is no Linux kernel and no Python dependency in the OS or build.

## NOL source tree

- src/boot.nol
- src/shell.nol
- src/help.nol
- src/system.nol
- src/terminal.nol

These are compiled together into the kernel.

## Build

The GitHub Action installs the bare-metal build tools, builds the tiny C bootstrap compiler, compiles every .nol file, links the kernel, creates the ISO, and performs a QEMU boot smoke test.

## Architecture

GRUB -> custom boot -> custom kernel -> NOL-generated OS logic -> terminal

The long-term goal is to remove the C translation step entirely:

NOL -> NOL IR -> native machine code

## Roadmap

1. NOL lexer/parser
2. NOL IR and native x86 backend
3. memory manager
4. interrupts and timer
5. processes and syscalls
6. filesystem
7. executable loader
8. USB
9. networking
10. optional x86_64/UEFI path
11. NOL standard library and package system

Third-party components remain separately credited and licensed in CREDITS.md.
