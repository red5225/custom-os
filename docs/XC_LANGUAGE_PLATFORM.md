# XC / XCUI language-first CustomOS architecture

## Non-negotiable language boundary

- **C** is reserved for the small kernel core and genuinely lowest-level hardware primitives.
- **XC** implements the language runtime/compiler, device drivers, filesystems, memory services, networking, USB/audio/graphics/input services, utilities, and applications wherever practical.
- **XCUI** defines desktop/UI structure, widgets, layout, styling, and event/action bindings. XCUI declarations lower into XC declarations and handler functions.

Pipeline: **XCUI → XC → C/native code**. The C backend is a bootstrap implementation detail, not a reason to author higher-level OS subsystems in C.

## Repository naming

- `c-...`: C-only kernel/low-level hardware directories.
- `xc-...`: XC compiler/runtime/libraries/drivers/services.
- `xcui-...`: XCUI parser/compiler/runtime.
- `examples/` and `tests/` are explicitly non-production support directories.

The language/platform areas are separated into `xc-language/`, `xc-drivers/`, `xc-services/`, and `xcui-runtime/`. The existing boot/kernel build paths are retained during the migration so the known UEFI path is not broken by a blind mass rename. Moving the existing kernel source into `c-kernel/` is a tracked migration task that must update and validate every build/linker reference before old paths are removed.

## Current implementation facts

- The UEFI loader and small freestanding kernel exist.
- The host XC interpreter supports a small starter grammar; it is not yet a complete systems language.
- An initial XC-to-C generator and smoke tests exist on the feature branch.
- The existing XCUI renderer is early-stage; event input and action dispatch into XC functions are still incomplete.
- No complete USB, Wi-Fi, audio, touchpad, storage, or graphics driver suite is claimed.

## Implementation sequence

1. Complete a real lexer, parser, AST, diagnostics, typed functions, control flow, modules/imports, and a checked C bootstrap backend.
2. Define and test a versioned kernel ABI for memory maps, allocation, logging, interrupts, device resources, and graphics surfaces.
3. Implement an XC driver registry and service lifecycle with permission-scoped device resources.
4. Implement XCUI lowering into XC handler declarations and connect mouse/keyboard events to those handlers.
5. Port subsystems to XC incrementally, with hardware-independent tests and QEMU integration tests.

A CI pass only proves the checks that actually ran; it does not establish physical Chromebook compatibility or completed subsystem support.