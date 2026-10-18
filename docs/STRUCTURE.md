# Source tree map

```text
boot/uefi/       UEFI entry point, firmware handoff and loader interfaces
kernel/          freestanding kernel core and architecture-specific code
drivers/         native hardware drivers; no Linux driver tree
memory/          physical/virtual memory management
interrupts/      exceptions, IDT, interrupt controllers and timers
storage/         block devices, partition readers and filesystems
console/         serial, text and framebuffer consoles
language/xc/     XC language grammar, compiler/runtime design and library
stdlib/xc/       XC standard library, split into focused modules
apps/example/    user-facing XC programs
tests/            host-side tests and fixtures
 tools/           host diagnostics and developer tools
```

This is the target organization. A folder should appear only when it contains real code or a concrete interface, not empty placeholder files. Existing milestone-one code remains under `milestone1/` until the native boot interfaces are deliberately migrated; do not move source files without updating and testing the build.
