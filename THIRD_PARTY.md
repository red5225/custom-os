# Third-party components

The native UEFI build uses the host toolchain and GNU-EFI headers/libraries at build time. These dependencies are not bundled into CustomOS output.

CustomOS is not a Linux-kernel fork. The native build must not bundle a Linux kernel, Linux root filesystem, Buildroot, or another Linux distribution. Future third-party code must be documented here with its upstream URL, version, license, and exact files used.
