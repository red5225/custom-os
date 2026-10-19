# XC system services

All ordinary OS services belong here in XC: memory allocation policy, process/task management, filesystems, networking, device discovery, logging, configuration, and IPC. These are not C kernel modules.

## Service rules

- Services consume typed XC interfaces and use the kernel only through the versioned low-level ABI.
- No service may assume that a device, filesystem, network, or allocation request succeeds.
- Public service APIs must document ownership, errors, concurrency, and resource limits.
- Each service needs unit tests and an integration test before being described as operational.

## Planned implementation order

1. Kernel ABI and memory-map handoff.
2. XC runtime allocation and error handling.
3. Device registry and service lifecycle.
4. Storage block API and a read-only FAT implementation.
5. PCI discovery and interrupt/event delivery.
6. USB, network, graphics, audio, and input services.

These are planned components, not currently working drivers.