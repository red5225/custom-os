# CustomOS UI milestone

## What this adds

- CustomOS-branded keyboard-driven home screen on Linux virtual terminal 1.
- System overview and kernel/command-line status screens.
- Credits screen.
- Maintenance shell, reboot and power-off menu actions.
- Serial boot console left available for diagnostics.

## Current limitations

This is a text-mode interface, not a graphical desktop. Mouse support, compositor/window manager, Wi-Fi, audio, touchpad, power management, Chromebook-specific drivers and physical CB314-3H boot are not claimed as working. QEMU testing is necessary but not sufficient for hardware validation.

The interface is launched by the CustomOS init script when /dev/tty1 exists. If a platform has no usable virtual terminal, the boot console remains the fallback.
