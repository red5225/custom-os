# Firmware Probe (Boot and image startup)

**Component ID:** 9

## Responsibility

This component defines a narrow interface for firmware probe in the boot and image startup area. It has a stable name, a unique catalog ID, initialization state, and a self-check so CI can detect broken interfaces early.

## Current maturity

This is a tested component-catalog scaffold. It is compiled and exercised independently by `scripts/test_component_catalog.py`; it is **not yet wired into the boot image or physical-device driver path**. Integration requires a platform implementation and explicit ownership in the kernel/build design.

## API

- `customos_firmware_probe_init()` — marks this component initialized.
- `customos_firmware_probe_capabilities()` — returns catalog ID 9.
- `customos_firmware_probe_name()` — returns the stable component key.
- `customos_firmware_probe_self_check()` — reports whether initialization and identity checks pass.
