# XC driver contract (implementation target)

This document defines the first driver subsystem API before individual drivers are added. It is deliberately separate from examples.

## Resource descriptor

A driver receives a descriptor created by the kernel, never a raw address guessed from device data. The descriptor must identify the resource kind, resource length, and a revocable capability. Physical addresses may be logged for diagnostics but are not a permission to dereference them.

## Lifecycle

The XC driver interface will expose these typed operations once XC functions/modules are implemented:

- `probe(device_info) -> Result<DriverInstance, ProbeError>` — match identifiers and validate metadata.
- `start(instance, resources) -> Result<(), DriverError>` — initialize only resources granted by the kernel.
- `handle_event(instance, event) -> Result<EventDisposition, DriverError>` — process bounded event data and validate every length.
- `stop(instance) -> Result<(), DriverError>` — stop device activity and release resources.
- `describe(instance) -> DriverStatus` — return safe diagnostic state.

These signatures are API design notation, not valid executable XC syntax in the current compiler. The current compiler does not yet support functions, structs, modules, or `Result`; the first driver implementation is blocked on those language features and the kernel ABI integration.

## Required properties

- Versioned interface and capability checks.
- No unchecked integer arithmetic on device lengths or offsets.
- No unbounded polling loops in event handlers.
- Explicit failure handling and teardown on partial initialization.
- Host-side tests for parsing/validation plus QEMU integration tests before hardware claims.
