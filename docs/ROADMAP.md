# Native CustomOS roadmap

This roadmap describes planned work, not completed features.

## Boot and kernel
- [ ] Make UEFI entry, memory-map capture, and kernel handoff deterministic.
- [ ] Add serial logging and explicit panic/error reporting.
- [ ] Add physical-memory map and page allocator with tests.
- [ ] Add IDT, exception handlers, interrupt controller setup, and timer.
- [ ] Add a stable driver interface and bounded hardware waits.
- [ ] Add block-device abstraction, partition parsing, and read-only filesystem support.
- [ ] Add framebuffer discovery and a basic console.
- [ ] Port keyboard and mouse support into the canonical native kernel.

## XC language
- [x] Implement a host-side interpreter for the initial `.xc` grammar.
- [x] Add CI smoke tests for arithmetic, conditions, and input.
- [ ] Document the XC grammar and error model in one concise reference.
- [ ] Add error cases, nested-condition tests, and more examples.
- [ ] Add a tested XC standard library of distinct modules.
- [ ] Design and implement XC runtime integration in the native kernel before claiming in-kernel execution.

## Quality rules
- Every source file must implement a distinct function, interface, test, fixture, or documented component.
- No generated filler, numbered placeholder modules, duplicate versions, or dead demos to inflate file count.
- Keep the boot chain independent of Linux; host build tools are not part of the OS image.
- Report actual tracked-file count and artifact size from CI.

## File-count goal
The 500+ tracked-file goal remains outstanding. Reach it through real subsystem modules, headers, tests, XC library modules, and hardware-independent fixtures. Do not create empty files merely to reach a number.

## Hardware caveat
The project is experimental. A QEMU boot test does not establish compatibility with the Acer Chromebook CB314-3H-C4V5 or its firmware/security configuration.
