# Kernel Log (Kernel core services)

**Component ID:** 18

## Responsibility

This component defines a narrow interface for kernel log in the kernel core services area. It has a stable name, a unique catalog ID, initialization state, and a self-check so CI can detect broken interfaces early.

## Current maturity

This is a tested component-catalog scaffold. It is compiled and exercised independently by `scripts/test_component_catalog.py`; it is **not yet wired into the boot image or physical-device driver path**. Integration requires a platform implementation and explicit ownership in the kernel/build design.

## API

- `customos_kernel_log_init()` — marks this component initialized.
- `customos_kernel_log_capabilities()` — returns catalog ID 18.
- `customos_kernel_log_name()` — returns the stable component key.
- `customos_kernel_log_self_check()` — reports whether initialization and identity checks pass.
