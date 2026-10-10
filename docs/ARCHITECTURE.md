# CustomOS architecture

## First milestone
- Buildroot builds the Linux kernel, toolchain and initramfs.
- BusyBox init/userspace plus Python 3 provide a usable terminal system.
- CustomOS tools provide identity, app-manifest validation and SHA-256 recovery prototype.
- GRUB ISO contains the kernel and compressed initramfs.
- GitHub Actions runs unit tests and a QEMU serial-console boot assertion.

## Next milestones
1. Wayland session and lightweight compositor with native shell.
2. File manager, settings, terminal, text editor and Python IDE.
3. App lifecycle, permissions, package validation and UI markup parser/renderer.
4. Signed integrity manifest and verified recovery source.
5. Physical Chromebook testing: firmware, display, input, Wi-Fi, audio, suspend and battery.

## Security
The prototype verifies hashes and can atomically restore from a recovery copy matching the expected hash. The manifest is not authenticated, so a privileged attacker could replace both files and manifest. This is not tamper-proof security.
