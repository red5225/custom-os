# Chromebook trackpad investigation (CustomOS)

Target: Acer Chromebook CB314-3H-C4V5, Intel Celeron N4500, board family reported as `magneto`, custom UEFI firmware.

## Findings

The current kernel reads legacy i8042 ports and decodes PS/2 auxiliary packets. That is not a valid assumption for the built-in Chromebook trackpad, and it should not be described as trackpad support. A successful keyboard path does not imply the touchpad shares that interface.

UEFI defines `EFI_SIMPLE_POINTER_PROTOCOL` for relative pointer devices, but it is a firmware boot-services interface. It is useful for detecting whether firmware exposes pointer input during pre-boot, but it is not a sound substitute for a kernel-owned driver after `ExitBootServices`.

For a native driver, the next steps are:

1. Capture the machine's actual ACPI tables and PCI device IDs on the Chromebook using a trusted existing OS / diagnostic environment.
2. Identify the touchpad's ACPI `_HID` / `_CID`, its I2C bus resources, interrupt GPIO and the I2C controller PCI ID.
3. Implement the matching I2C controller path, ACPI resource parsing, HID-over-I2C device descriptor and report-descriptor parsing, plus pointer event delivery.
4. Test movement, click, tap-to-click and release behavior on the physical CB314-3H-C4V5; separately test QEMU with a supported virtual pointer.

The Linux ACPI enumeration documentation explains that I2C peripherals are described using ACPI resources and that the I2C controller must be registered before attached devices can be enumerated. Intel's THC documentation describes one specific Intel touch-controller family; it is not proof that this exact Chromebook uses THC. Do not hard-code a controller or touchpad model until the hardware IDs and ACPI tables are collected.

## References

- UEFI 2.10 Simple Pointer Protocol: https://uefi.org/specs/UEFI/2.10/12_Protocols_Console_Support.html
- Linux ACPI device enumeration: https://docs.kernel.org/firmware-guide/acpi/enumeration.html
- Linux Intel Touch Host Controller documentation (controller-family reference only): https://docs.kernel.org/hid/intel-thc-hid.html

## Status

**Not implemented yet:** native I2C-HID touchpad support. The current PS/2 auxiliary code remains emulator/legacy-device support only. No claim of physical Chromebook trackpad support is made until tested on the target hardware.
