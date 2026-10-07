# XCL

**XCL — eXecutable Core Language** — is the primary source language of CustomOS.

The OS-facing shell, boot text, help system, status layer, and terminal layer live in `.xcl` files.

The current bootstrap compiler (`tools/xclc.c`) translates XCL into freestanding C during the build. This is temporary: the roadmap is XCL parsing -> XCL IR -> native machine code, followed by a self-hosted compiler.

XCL is intended to become the main implementation language for CustomOS, with only the lowest-level boot and hardware boundaries remaining in C/assembly.
