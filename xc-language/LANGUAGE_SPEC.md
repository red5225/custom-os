# XC language specification target

Status: design contract; not all features below are implemented yet.

## Source and modules

- Source extension: `.xc`.
- UTF-8 source text.
- Modules have explicit names and imports; imports must resolve deterministically and circular imports must be diagnosed.
- Public declarations are explicit. Internal names are private by default.

## Required core types

- `bool`, signed and unsigned fixed-width integers, `usize`.
- `str` as a length-aware UTF-8 string view; no implicit C-string assumptions.
- Fixed arrays, slices, records/structs, enums, option/result types.
- Explicit pointers and unsafe blocks only for code granted low-level capability.

## Required statements and expressions

- `let` and typed declarations; assignment.
- Functions with typed parameters and return values.
- `if`/`else`, `while`, bounded `for`, `break`, `continue`, `return`.
- Calls, field access, indexing, arithmetic, comparisons, boolean operators.
- Pattern matching for enums and result values.

## Runtime and safety rules

- Integer overflow behavior is defined and testable; no silent undefined behavior.
- Array/slice bounds are checked unless an explicitly unsafe operation is used.
- Strings carry lengths and are not required to be NUL-terminated.
- Allocation APIs return explicit failure results; ownership/lifetime rules must be documented and compiler-enforced where possible.
- Kernel calls and port/MMIO access require explicit capability-bearing APIs. Normal XC modules cannot issue arbitrary hardware I/O.
- Errors preserve source locations through lowering and code generation.

## Backends

The bootstrap backend emits portable C11 and compiles it with a host C compiler for tests. OS-target builds must use a freestanding target configuration and a reviewed kernel ABI. The C backend is an implementation strategy, not permission to move drivers/services into C.

## Acceptance tests

Every feature needs positive tests, negative syntax/type tests, deterministic diagnostics, and at least one generated-code execution test when applicable. Files under `tests/` are tests only and are not counted as implementations.