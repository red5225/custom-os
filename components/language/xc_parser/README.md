# Xc Parser (XC language and runtime)

**Component ID:** 72

## Responsibility

This component defines a narrow interface for xc parser in the xc language and runtime area. It has a stable name, a unique catalog ID, initialization state, and a self-check so CI can detect broken interfaces early.

## Current maturity

This is a tested component-catalog scaffold. It is compiled and exercised independently by `scripts/test_component_catalog.py`; it is **not yet wired into the boot image or physical-device driver path**. Integration requires a platform implementation and explicit ownership in the kernel/build design.

## API

- `customos_xc_parser_init()` — marks this component initialized.
- `customos_xc_parser_capabilities()` — returns catalog ID 72.
- `customos_xc_parser_name()` — returns the stable component key.
- `customos_xc_parser_self_check()` — reports whether initialization and identity checks pass.
