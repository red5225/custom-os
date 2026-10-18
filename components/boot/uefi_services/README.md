# Uefi Services (Boot and image startup)

**Component ID:** 2

## Responsibility

This component defines a narrow interface for uefi services in the boot and image startup area. It has a stable name, a unique catalog ID, initialization state, and a self-check so CI can detect broken interfaces early.

## Current maturity

This is a tested component-catalog scaffold. It is compiled and exercised independently by `scripts/test_component_catalog.py`; it is **not yet wired into the boot image or physical-device driver path**. Integration requires a platform implementation and explicit ownership in the kernel/build design.

## API

- `customos_uefi_services_init()` — marks this component initialized.
- `customos_uefi_services_capabilities()` — returns catalog ID 2.
- `customos_uefi_services_name()` — returns the stable component key.
- `customos_uefi_services_self_check()` — reports whether initialization and identity checks pass.
