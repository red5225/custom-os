# XC / XCUI language-first CustomOS architecture

## Intended boundary

CustomOS should be implemented in XC and XCUI wherever practical. C is reserved for the small kernel core and hardware-facing primitives that require direct machine-level control. C should not be the default implementation language for drivers, filesystems, networking services, the desktop, or applications.

- **C kernel / hardware ABI:** boot handoff, interrupt entry/exit, CPU setup, low-level memory primitives, port/MMIO access, DMA setup, and narrowly scoped hardware operations.
- **XC:** device-driver policy and state machines, filesystems, networking protocols, system services, package tools, command-line programs, and application logic.
- **XCUI:** UI trees, layout, theme/style, windows, controls, accessibility labels, and event/action bindings.
- **Compilation:** XCUI lowers into XC UI declarations and action bindings; XC is type-checked and lowered to generated C or a future direct native backend; the target compiler/linker produces machine code.

## Current status

This document describes the target architecture, not a claim that all these subsystems already exist. The current XC interpreter supports a small statement set, and the current XCUI parser/runtime is an early implementation. The initial tools/xc/xc_codegen.cpp compiler emits C for the starter subset: program, integer/string let, say, show, add, ask, equality if/else/end, and finish. It is not yet a complete compiler or a driver-capable language.

## Compiler pipeline

1. Parse XC into a syntax tree and report source locations for errors.
2. Resolve names, types, imports, function signatures, and target capabilities.
3. Lower XCUI components and events into XC UI declarations and XC action functions.
4. Lower XC into a target-neutral intermediate representation, with checked arithmetic and explicit memory/ownership rules.
5. Emit C for the current backend, compile with a freestanding target toolchain where required, and link against the XC runtime and kernel ABI.
6. Run host unit tests, compiler golden tests, freestanding compile checks, and QEMU integration tests in CI.

The starter compiler is deliberately a first milestone. The existing interpreter remains available for current examples while the compiled pipeline grows.

## Driver model

An XC driver should be an XC module that registers a device with the kernel through a small C ABI. The ABI should expose opaque handles and bounded operations such as device discovery, MMIO-region mapping, interrupt registration, DMA-buffer allocation, and safe I/O requests. It must not expose arbitrary kernel memory or allow an untrusted driver to write arbitrary physical addresses.

A driver module should declare device IDs, required capabilities, initialization, interrupt/event handling, reset, and shutdown. The kernel owns privilege, interrupt entry, page tables, and hardware primitives; XC owns protocol logic, queues, state machines, retries, and higher-level device behavior.

## Roadmap

- [x] Define the language-first architecture and boundary.
- [x] Add an initial XC-to-C compiler for the existing starter syntax.
- [ ] Add parser/AST, type checking, functions, parameters, return values, loops, modules/imports, and standard-library semantics.
- [ ] Add a real XCUI-to-XC lowering tool with source maps and diagnostics.
- [ ] Add XC UI event handlers and bindings that call named XC functions.
- [ ] Define and version the kernel hardware ABI.
- [ ] Implement the first XC driver against a mock hardware ABI, with unit tests.
- [ ] Port device drivers and services incrementally; keep only genuinely low-level primitives in C.
- [ ] Add freestanding cross-compilation and QEMU tests for compiled XC modules.
