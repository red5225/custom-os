# XCUI — UI language and C runtime for CustomOS

XCUI is a declarative UI language, host-side validator/compiler, and freestanding C runtime for red5225/custom-os. It is not an IDE or desktop application.

The language describes interface structure. XC remains the logic language. XCUI buttons carry action identifiers that the OS can dispatch to XC through a narrow C callback. The CustomOS repository has been inspected read-only; no CustomOS files were changed here.

## Build and test

Requires a C11 compiler and Make. On Intel macOS Monterey, install Apple's Command Line Tools, then run:

    make clean test example

## Example

    app "CustomOS Shell" {
        window "Desktop" background "#151b26" {
            row gap 12 padding 16 {
                panel "Sidebar" width 220 background "#202b3d" {
                    text "CustomOS" size 24 color "#ffffff"
                    button "Files" action "open_files" height 40
                    button "Settings" action "open_settings" height 40
                }
                column gap 8 padding 12 {
                    text "Welcome to CustomOS" size 28 color "#ffffff"
                    text "Native UI described by XCUI." color "#b8c4d8"
                    row gap 8 {
                        button "Terminal" action "open_terminal" height 44
                        button "Network" action "open_network" height 44
                    }
                }
            }
        }
    }

## Compile XCUI into linkable C data

    build/xcui check examples/desktop.xcui
    build/xcui build examples/desktop.xcui -o build/desktop_embedded.c

The compiler parses and validates XCUI on the host, then emits a C initializer named xcui_embedded_tree. Link that generated file with src/xcui_runtime.c and the OS drawing/input adapters. The target does not need to parse XCUI text at boot.

## Runtime API

- xcui_layout(tree, screen_width, screen_height) computes pixel bounds.
- xcui_render(tree, backend, context) calls OS-provided rectangle and text drawing callbacks.
- xcui_dispatch_pointer(tree, x, y, dispatcher, context) hit-tests buttons and forwards action identifiers to XC's action registry.
- xcui_hit_test and xcui_node_action are available for custom input handling.

The runtime uses fixed-size structures and no heap allocation. The host parser and command-line tool are separate from src/xcui_runtime.c, which can be compiled freestanding. The renderer is an early foundation, not yet a complete compositor: font rasterization, focus traversal, keyboard navigation, accessibility, reactive state, and a full window manager remain OS-side work.

## XC integration status

The action callback is the C bridge point, but the current XC interpreter does not yet have native UI statements or automatic action registration. A CustomOS patch must register XC functions and connect its input events to the bridge. This repository intentionally does not modify CustomOS.

## Limits and safety

- 128 nodes maximum; nesting limit 32.
- Initial components: window, screen, row/hstack, column/vstack, panel, text, label, button, spacer, divider.
- Initial properties: width, height, padding, gap, size, color, background, action.
- Keep parsing out of privileged kernel mode; compile to validated C data on the host.
- No successful Chromebook boot or complete CustomOS integration is claimed yet.


## CustomOS integration

This source is vendored into `red5225/custom-os/ui/xcui`. The host build validates `examples/desktop.xcui`, generates a static C tree, and links the freestanding layout/render runtime into the native kernel. The kernel renders the embedded desktop through GOP framebuffer callbacks. Interactive XC action dispatch still requires the OS input loop and XC action registry; it is not claimed as complete.
