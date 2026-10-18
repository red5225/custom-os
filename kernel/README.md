# Native kernel core

The current freestanding kernel source is `milestone1/kernel.c`. `milestone1/kernel.ld` defines its required memory layout and is necessary build code, not junk. Move it into this folder only together with the Makefile, boot protocol, and tests that consume it.
