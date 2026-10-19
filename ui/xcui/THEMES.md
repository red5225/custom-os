# CustomOS XCUI themes

The desktop is described in `examples/desktop.xcui`. The `themes/` folder contains copyable color presets:

- `light.xcui` — soft white and grey, blue accent.
- `dark.xcui` — dark navy, light text, blue accent.
- `blue.xcui` — pale blue canvas with restrained blue accents.

## Change the OS colors

Edit `background` on the `window` for the desktop canvas, `background` on `panel` for the app surface, and `color` on `text` for labels. Button fills use the runtime default in `ui/xcui/src/xcui_runtime.c` until a per-button background property is added. Keep `height` on each child in a `column` and `height` on rows/buttons: the current runtime uses explicit heights to avoid stretching labels across the screen.

To try a preset, copy its contents into `examples/desktop.xcui` and push. The existing CI workflow validates and embeds that one file. The theme presets are source templates, not a runtime theme switcher yet.

## Current limits

This is an early framebuffer UI: no rounded corners, font antialiasing, window compositor, drag-and-drop, or saved user preferences yet. The boot splash now has a simple geometric CustomOS mark and a longer fixed delay. The longer delay is a bounded busy wait, not a real timer.
