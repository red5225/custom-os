# Task Metrics (Process and task services)

**Component ID:** 38

## Responsibility

This component defines a narrow interface for task metrics in the process and task services area. It has a stable name, a unique catalog ID, initialization state, and a self-check so CI can detect broken interfaces early.

## Current maturity

This is a tested component-catalog scaffold. It is compiled and exercised independently by `scripts/test_component_catalog.py`; it is **not yet wired into the boot image or physical-device driver path**. Integration requires a platform implementation and explicit ownership in the kernel/build design.

## API

- `customos_task_metrics_init()` — marks this component initialized.
- `customos_task_metrics_capabilities()` — returns catalog ID 38.
- `customos_task_metrics_name()` — returns the stable component key.
- `customos_task_metrics_self_check()` — reports whether initialization and identity checks pass.
