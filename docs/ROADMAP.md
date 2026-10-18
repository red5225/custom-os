# CustomOS implementation roadmap

This roadmap tracks work that should become real, tested code. It is not a promise that unimplemented hardware works.

## Phase 1 — native UEFI kernel

- [ ] Choose one boot path as the canonical path and keep the Linux/Buildroot image clearly separated as a temporary experiment.
- [ ] UEFI entry, memory-map capture, page allocator, physical-memory map, and panic path.
- [ ] Serial logging and deterministic QEMU smoke tests.
- [ ] Interrupt descriptor table, exception handlers, and timer.
- [ ] Keyboard and mouse input with bounded waits and explicit initialization errors.
- [ ] Block-device abstraction and read-only partition/filesystem inspection.
- [ ] XC language: documented grammar, parser/compiler tests, error messages, and executable examples.
- [ ] C++ host diagnostics and C# manifest validation remain host tools, not kernel dependencies.

## Quality gates

- Every new module must be used by a build target or test.
- Every driver needs initialization, timeout/error handling, and tests where hardware-independent testing is possible.
- Every public interface needs a header and at least one test or documented consumer.
- Do not create numbered placeholder files merely to increase the file count.
- Report the actual tracked-file count and ISO size from CI; never infer either from a target.

## File-count target

The desired 500+ tracked files is a future scale target, not a reason to manufacture empty modules. Reach it through real subsystem source, headers, driver tests, XC standard-library modules, and fixtures as those features are implemented. The current repository is much smaller and does not yet meet that target.

## Build architecture warning

The current integrated ISO workflow still uses a Linux kernel and Buildroot root filesystem. That is not the same as a fully original kernel. The `milestone1/` UEFI experiment is separate until the build, boot protocol, and tests are deliberately unified. Do not describe the Chromebook as supported until hardware testing verifies it.
