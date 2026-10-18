# Xc Module Loader (XC language and runtime)

**Component ID:** 80

## Responsibility

This component defines a narrow interface for xc module loader in the xc language and runtime area. It has a stable name, a unique catalog ID, initialization state, and a self-check so CI can detect broken interfaces early.

## Current maturity

This is a tested component-catalog scaffold. It is compiled and exercised independently by `scripts/test_component_catalog.py`; it is **not yet wired into the boot image or physical-device driver path**. Integration requires a platform implementation and explicit ownership in the kernel/build design.

## API

- `customos_xc_module_loader_init()` — marks this component initialized.
- `customos_xc_module_loader_capabilities()` — returns catalog ID 80.
- `customos_xc_module_loader_name()` — returns the stable component key.
- `customos_xc_module_loader_self_check()` — reports whether initialization and identity checks pass.
