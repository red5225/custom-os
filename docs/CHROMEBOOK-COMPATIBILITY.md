# Chromebook compatibility report

Status date: 2026-10-10.

- User-stated target: Acer Chromebook CB314-3H.
- Build target: generic x86-64 first; confirm exact SKU from the device label/system information.
- Firmware mode: unknown.
- Managed status: unknown. Do not modify firmware or security settings on a school/organization-managed device without administrator authorization.

Confirmed: generic x86-64 Linux build target and QEMU first validation gate.

Not physically tested: CB314-3H firmware boot, graphics acceleration, Wi-Fi/Bluetooth, audio, touchpad, special keys, suspend/resume, battery, exact wireless chipset and firmware.

Firmware modifications can make a Chromebook unbootable and affect data/security. This project does not modify firmware. First test in QEMU; use USB only if already-authorized firmware supports external boot.
