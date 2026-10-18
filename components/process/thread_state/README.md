# Thread State (Process and task services)

**Component ID:** 33

## Responsibility

This component defines a narrow interface for thread state in the process and task services area. It has a stable name, a unique catalog ID, initialization state, and a self-check so CI can detect broken interfaces early.

## Current maturity

This is a tested component-catalog scaffold. It is compiled and exercised independently by `scripts/test_component_catalog.py`; it is **not yet wired into the boot image or physical-device driver path**. Integration requires a platform implementation and explicit ownership in the kernel/build design.

## API

- `customos_thread_state_init()` — marks this component initialized.
- `customos_thread_state_capabilities()` — returns catalog ID 33.
- `customos_thread_state_name()` — returns the stable component key.
- `customos_thread_state_self_check()` — reports whether initialization and identity checks pass.
