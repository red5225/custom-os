# CustomOS hardware target: Acer Chromebook CB314-3H-C4V5

Status: research inventory, not a claim that these devices are already supported by the kernel.

## Confirmed target facts

- Product: Acer Chromebook 314, model CB314-3H-C4V5 (user-provided exact SKU).
- Architecture: x86-64.
- CPU: Intel Celeron N4500 (user-provided and consistent with Acer CB314-3H family specifications).
- Graphics: Intel UHD Graphics integrated into the N4500.
- Board family: `magneto` (user-provided firmware/device information; some public ChromeOS references spell the family `magento`, so the firmware-reported board identity should be treated as authoritative).
- Firmware version reported by user: `152.16765.0`; firmware state reported as `custom`.
- Memory: LPDDR4x soldered to the board; exact capacity for this individual unit has not been verified here.
- Storage: eMMC integrated on the mainboard; capacity for this unit has not been verified here.
- Display: internal LCD using an eDP cable; exact panel and resolution should be verified from firmware/device data or panel label.

## Chassis-level components documented for the CB314-3H family

Acer's CB314-3H Lifecycle Extension Guide identifies these replaceable/connected assemblies: battery pack, mainboard, separate USB board, keyboard and touchpad flat cables, WLAN module and antenna leads, speakers, LCD/eDP cable, camera/microphone in the display assembly, and a G-sensor board/cable where fitted. Connector count and assembly variants can differ by SKU/revision.

## Chip identity confidence

| Component | Current identification | Confidence | Next evidence needed |
|---|---|---|---|
| CPU / iGPU | Intel Celeron N4500 / integrated Intel UHD Graphics | High for target configuration | CPUID and PCI device ID at boot |
| Mainboard | CB314-3H family; user-reported board family `magneto` | Medium-high | Printed PCB model/revision and full board photo |
| System RAM | LPDDR4x, soldered | High for family | Firmware memory map / SMBIOS if exposed |
| Internal storage | eMMC, soldered on board | High for family | PCI/ACPI enumeration and eMMC controller/device IDs; read-only diagnostics |
| WLAN / Bluetooth | Exact module not confirmed | Unknown | Printed module label or PCI/USB hardware IDs. Do not assume Intel AX201 from other CB314-3H SKUs. |
| USB host controller | Exact controller and port routing not confirmed | Unknown | PCI class/vendor/device IDs and ACPI tables |
| USB daughterboard | Separate board with 20-pin and 50-pin FFCs documented in family guide | High for family | Visual check against the exact unit; do not unlock/remove FFCs just to identify it |
| Touchpad | Module plus FFC documented | High for family | I2C/HID/ACPI identity from firmware or device enumeration |
| Keyboard | Keyboard FPC documented | High for family | Firmware/ACPI event path and scan-code testing |
| Audio | Speakers documented; codec/controller model unknown | Low for chip model | PCI IDs, ACPI, codec enumeration |
| Display panel | Internal eDP panel | High for interface | EDID/panel label for exact model and timing |
| Camera/microphone | Present in family design; exact sensor unknown | Medium | USB/I2C IDs or module labels |
| G-sensor | Separate board listed in family guide where fitted | Medium | ACPI/I2C enumeration and exact-SKU inspection |
| Battery / charging | Battery pack and USB-C charging hardware | High for broad function; controller IDs unknown | ACPI/EC telemetry and power-controller identification |
| Firmware security / TPM | Chromebook firmware/security-controller behavior is platform-specific | Unknown exact chip | ACPI/TPM device IDs; do not modify firmware protection to identify it |

## Image-search findings and limits

Public image results include CB314-1H/CB314-3H replacement boards and photos of related SKUs. Those images are useful for orientation but do not prove that the photographed board is the C4V5 board revision. Some replacement listings show a different processor (for example N5030), so their chip layout and Wi-Fi module must not be treated as exact evidence for this N4500 device. Acer lists Wi-Fi 6 AX201 for at least one other CB314-3H SKU; that is not sufficient to assert AX201 in the C4V5.

## Driver implementation plan (from scratch; no Linux kernel)

1. Boot diagnostics: print CPU CPUID, firmware tables (ACPI), memory map, PCI configuration-space devices and vendor/device/class IDs.
2. Platform foundation: exception/interrupt handling, APIC/timer, ACPI parser, PCI enumeration and safe device registry.
3. Input: PS/2 fallback if exposed, then ACPI/I2C-HID or USB-HID keyboard/touchpad paths based on actual IDs.
4. Storage: identify the actual eMMC controller path first. Implement the matching controller and eMMC command protocol, then a read-only block layer and partition/filesystem parser; enable writes only after read paths and bounds checks are tested.
5. USB: identify host controller; implement xHCI if present, then USB enumeration, hubs, HID and mass-storage BOT/SCSI. Never report USB storage support before device-level tests pass.
6. Networking: identify WLAN PCI/USB IDs; verify firmware requirements and licensing; implement the actual bus/driver plus 802.11 and network stack. No guessed driver.
7. Display/audio/camera/power: use exact IDs and ACPI description to select controller-specific work; begin with framebuffer output and safe telemetry.
8. XC/XCUI: connect input events and UI actions to the kernel only after the relevant event paths are implemented and regression-tested.

## Safety and validation rules

- Never use a related model's photo or SKU as proof of the exact chip.
- Do not write to or format the internal eMMC during identification.
- Do not alter firmware write protection as a prerequisite for driver discovery.
- Every subsystem must distinguish `detected`, `initialized`, `functional`, and `tested`; detection is not driver support.
- QEMU tests validate generic code paths only, not compatibility with this Chromebook's physical firmware or components.

## References

- Acer CB314-3H family specifications: https://www.acer.com/in-en/chromebooks/acer-chromebook-314-cb314-3h-cb314-3ht/pdp/NX.K04SI.008
- Acer CB314-3H lifecycle/service guide: https://www.manualowl.com/m/Acer%20Computers/Chromebook-314-CB314-3H/Manual/665348
- CB314-3H device-family overview (use as secondary evidence, not exact-SKU proof): https://chromeos.guide/ChromeOS-Devices/Acer/Acer-Chromebook-314-%28CB314-3H%29/
