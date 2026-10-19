# C kernel and low-level hardware boundary

This is the explicitly `c-`-prefixed home for C-only kernel code and the lowest-level hardware primitives. The current boot build still compiles the original `kernel/` paths; those files will be migrated here only with a coordinated build/linker update and CI validation, not by copying them and pretending the migration is complete.

`include/c-kernel-abi.h` starts a versioned contract for the future XC platform boundary. It is not wired into boot yet. Drivers, filesystems, networking, and normal system services must not be added here; they belong in `xc-drivers/` and `xc-services/`.