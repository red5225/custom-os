# Capability Policy (Security and integrity services)

**Component ID:** 96

## Responsibility

This component defines a narrow interface for capability policy in the security and integrity services area. It has a stable name, a unique catalog ID, initialization state, and a self-check so CI can detect broken interfaces early.

## Current maturity

This is a tested component-catalog scaffold. It is compiled and exercised independently by `scripts/test_component_catalog.py`; it is **not yet wired into the boot image or physical-device driver path**. Integration requires a platform implementation and explicit ownership in the kernel/build design.

## API

- `customos_capability_policy_init()` — marks this component initialized.
- `customos_capability_policy_capabilities()` — returns catalog ID 96.
- `customos_capability_policy_name()` — returns the stable component key.
- `customos_capability_policy_self_check()` — reports whether initialization and identity checks pass.
