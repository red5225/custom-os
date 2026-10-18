# Image Layout (Boot and image startup)

**Component ID:** 4

## Responsibility

This component defines a narrow interface for image layout in the boot and image startup area. It has a stable name, a unique catalog ID, initialization state, and a self-check so CI can detect broken interfaces early.

## Current maturity

This is a tested component-catalog scaffold. It is compiled and exercised independently by `scripts/test_component_catalog.py`; it is **not yet wired into the boot image or physical-device driver path**. Integration requires a platform implementation and explicit ownership in the kernel/build design.

## API

- `customos_image_layout_init()` — marks this component initialized.
- `customos_image_layout_capabilities()` — returns catalog ID 4.
- `customos_image_layout_name()` — returns the stable component key.
- `customos_image_layout_self_check()` — reports whether initialization and identity checks pass.
