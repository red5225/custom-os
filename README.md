# CustomOS

CustomOS is a small terminal-first operating system built around its own language: XCL (eXecutable Core Language).

XCL is the main language. The OS-facing source is now .xcl under src/.

C and assembly are intentionally limited to boot, hardware primitives, and the XCL bootstrap boundary.

There is no Linux kernel and no Python dependency in the OS or GitHub Actions build.

Architecture: GRUB -> custom boot -> custom kernel -> XCL -> terminal.

Eventually the bootstrap compiler will be replaced with an XCL compiler that emits native machine code.
