# XCL bootstrap language (legacy kernel-build DSL)

This small `.xcl` syntax is used only to generate C shell functions for the freestanding Limine kernel. It is **not** the same grammar as XC (`.xc`) and is not a general-purpose compiler. The current bootstrap compiler is `tools/xclc.c`; it emits `kernel/xcl_generated.c` during `make`.

Supported forms are intentionally limited: `fn name()`, `fn command(cmd)`, `print "text";`, `call name;`, command comparisons such as `if cmd == "help" {`, `else if`, `else`, `clear;`, `backspace;`, and `return;`. Unsupported statements must fail the build rather than silently becoming no-ops.

**Migration status:** XCL is retained temporarily because the current freestanding kernel build depends on it. XC in `docs/XC-LANGUAGE.md` is the custom script language for user-space examples. The project should converge these into one grammar before deleting or renaming the kernel bootstrap files; the old NOL generation is removed.
