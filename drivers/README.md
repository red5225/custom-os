# CustomOS drivers

This directory is for original, freestanding driver code for the CustomOS kernel. It is not a Linux driver tree and does not bundle a Linux kernel.

## Current status

The repository is an early boot milestone. Do not infer hardware support from the existence of a folder or a hardware inventory document. Each driver must report whether its device was detected, initialized, functionally exercised, and tested on physical hardware.

The exact Acer Chromebook target inventory and confidence levels are maintained in [`docs/hardware-target.md`](../docs/hardware-target.md).

## Implementation order

1. PCI configuration-space enumeration and device registry.
2. ACPI/platform discovery and interrupt/timer infrastructure.
3. Actual internal eMMC controller path, with read-only block access first.
4. USB host controller and USB-HID / mass-storage support based on enumerated IDs.
5. Keyboard/touchpad event integration into the terminal and XCUI.
6. Wi-Fi/Bluetooth after identifying the exact WLAN module and firmware requirements.
7. Audio, camera, display enhancements, sensors, and battery/charging telemetry.

## Driver status vocabulary

- **Planned** — no implementation.
- **Detected** — hardware identity read, no working driver implied.
- **Initialized** — controller setup succeeds.
- **Functional** — basic device operations pass tests.
- **Hardware-tested** — exercised on the named physical model, with test details recorded.

Never claim a subsystem is functional based only on successful compilation or QEMU boot.