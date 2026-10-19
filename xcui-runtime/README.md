# XCUI runtime and compiler

XCUI is the UI declaration language. It must compile/lower into XC declarations and XC event handlers; it is not a separate OS language with a parallel implementation of services.

## Required pipeline

`.xcui` source → XCUI parser/validator → typed UI tree + XC handler declarations → XC compiler → C/native backend.

## Runtime responsibilities

- Build a validated tree of windows, panels, labels, buttons, inputs, menus, and dialogs.
- Apply layout constraints and clipping consistently.
- Render through a narrow graphics surface API supplied by the XC platform.
- Convert mouse, keyboard, and accessibility input into typed UI events.
- Dispatch events to named XC functions and return results/errors to the UI.
- Keep widget declarations separate from OS service implementation.

## Current status

The existing renderer is an early native rendering path. A complete parser-to-XC pipeline, input event loop, and action dispatch are not yet claimed as implemented. New XCUI-only files belong in this `xcui-` directory.