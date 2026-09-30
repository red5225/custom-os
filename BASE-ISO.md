# Custom OS boot base

The current boot base is Tiny Core Linux 17.1 x86.

The uploaded Core-current.iso was inspected locally:
- 20,658,176 bytes
- ISOLINUX 4.05 El Torito boot image
- 4-sector no-emulation boot entry
- Linux 6.18.35-tinycore x86 kernel
- core.gz initramfs

The build verifies the downloaded base against the SHA-256 of the supplied ISO before producing build/os.iso.

This is intentionally a known-good booting base. The custom OS code can be layered on top after UTM boot is confirmed.
