# Chromebook trackpad investigation (CustomOS)

Target: Acer Chromebook CB314-3H-C4V5, Intel Celeron N4500, board family reported as `magneto`, custom UEFI firmware.

## Current input status

The kernel polls legacy i8042 keyboard input and attempts to initialize a PS/2 auxiliary mouse. This is useful for QEMU or a controller that exposes a compatible PS/2 mouse. It is **not proof that the built-in Chromebook trackpad works**. If the mouse works in QEMU but not on the Chromebook, that is consistent with the target trackpad using a different transport.

UEFI defines `EFI_SIMPLE_POINTER_PROTOCOL` for relative pointer devices, but it is a firmware boot-services interface. It can help diagnose whether firmware exposes pointer input before `ExitBootServices`; it is not a replacement for a kernel-owned driver afterwards.

## Required hardware evidence

Before implementing a controller-specific driver, collect the target machine's ACPI tables and PCI IDs using a trusted existing OS or diagnostic environment. Identify the touchpad ACPI `_HID` / `_CID`, I2C bus resources, interrupt GPIO, and I2C controller PCI ID. Then implement the matching I2C controller path, ACPI resource parsing, HID-over-I2C descriptor/report parsing and pointer event delivery. Test movement, clicks, tap-to-click and release behavior on the physical device.

Do not guess a controller or touchpad model from another Chromebook. Until the hardware IDs are known and the driver is tested on the CB314-3H-C4V5, native trackpad support remains **not implemented**. Keyboard navigation remains the fallback.

## References

- UEFI 2.10 Simple Pointer Protocol: https://uefi.org/specs/UEFI/2.10/12_Protocols_Console_Support.html
- Linux ACPI device enumeration: https://docs.kernel.org/firmware-guide/acpi/enumeration.html
- Linux Intel Touch Host Controller documentation (controller-family reference only): https://docs.kernel.org/hid/intel-thc-hid.html
