# CustomOS XCUI desktop design

The desktop is an original CustomOS implementation with a macOS-inspired menu-bar / dock-era desktop feel: restrained light surfaces, a blue accent, compact top navigation and a central app surface. It does not use Apple source code, icons, fonts or other proprietary assets.

## Edit the design

The main source is `examples/desktop.xcui`. The `themes/` files are copyable color/layout templates, not runtime theme switching. Colors use `#RRGGBB`; keep explicit heights on children in columns and rows because the current runtime distributes available space among children without a declared size.

## Runtime limitations

The current framebuffer renderer draws rectangular surfaces and bitmap text. It does not yet support rounded corners, transparency, a compositor, draggable/resizable windows, or real menu-bar actions. The top navigation labels are visual placeholders until menu actions are implemented. App screens are status pages: Files has no mounted storage backend, Terminal has no kernel shell, Settings are not persisted, and Network has no working adapter/TCP stack.

## Pointer status

The kernel currently polls a legacy PS/2 auxiliary mouse when firmware/emulator exposes it. That does not establish support for the built-in CB314-3H-C4V5 trackpad, which may require I2C-HID and ACPI-described resources. See `docs/chromebook-trackpad.md`; do not claim the Chromebook trackpad is fixed until the actual hardware IDs are collected and the driver is tested on-device.
