# Native drivers

Only add drivers that target the native kernel's own interfaces. Linux kernel driver sources and Linux-specific APIs are out of scope. Planned modules include PS/2 input where hardware permits, PCI enumeration, storage, timer, framebuffer, and serial. Every driver needs bounded waits, explicit errors, and a test or a documented hardware test procedure.
