# XC driver subsystem

This is the implementation home for device drivers written in XC. The existing `drivers/README.md` is historical project documentation; new driver code belongs here under an `xc-`-prefixed directory.

## Driver boundary

An XC driver must not directly access arbitrary physical addresses or I/O ports. It receives a versioned kernel capability/interface for the specific device resources it owns. Drivers must report probe/start/stop failures, validate device-provided lengths and identifiers, and support teardown.

## Required driver lifecycle

1. `probe`: match hardware identifiers and validate the supplied resource descriptor.
2. `start`: request only declared resources and initialize the device.
3. `poll` or event callback: process bounded work and validate every device-produced length.
4. `stop`: mask/stop device activity and release resources.
5. `status`: expose diagnostic state without leaking raw kernel addresses.

## Initial implementation status

This directory currently defines the XC driver contract, not a claim that USB, Wi-Fi, audio, touchpad, storage, or graphics drivers are complete. The first real driver should be implemented after the versioned kernel-to-XC ABI and XC function/module support are available.