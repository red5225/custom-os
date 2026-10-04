# Custom OS

A text-only x86_64 Linux OS built with Buildroot for UTM and the Acer Chromebook 314 CB314-3H family.

## Included
- Linux kernel with Intel Wi-Fi support
- linux-firmware Intel Wi-Fi firmware, including AX201/QuZ firmware
- ConnMan + wpa_supplicant + iw for networking
- Python 3
- pip via `python3 -m pip`
- USB, keyboard, touchpad/input and common storage/network drivers
- No custom graphical UI or framebuffer desktop

The CB314-3H family uses Intel Celeron N4500/N5100 variants; Acer lists Intel Wi-Fi 6 AX201 on several CB314-3H configurations. The Linux iwlwifi driver supports AX201.

## Build
GitHub Actions downloads the pinned Buildroot 2026.08 release, builds the Linux kernel and userspace, and produces `custom-os.iso`.

## Network
After boot, use `connmanctl` to enable Wi-Fi, scan, list services, and connect. `iw dev` and `ip link` are also available.

## Python
`python3` and pip are preinstalled. `python` aliases to `python3`, and `pip` aliases to `python3 -m pip`.
