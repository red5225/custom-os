# Freestanding kernel

`kernel.c` is the real freestanding x86-64 kernel entry point. It initializes serial output, validates boot information, and draws a basic status screen when GOP framebuffer data is available. `kernel.ld` defines the physical load layout; it is required linker configuration, not disposable documentation.
