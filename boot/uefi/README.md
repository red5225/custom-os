# UEFI boot path

The current verified source of truth for the experimental native UEFI build remains `milestone1/loader.c`, `milestone1/kernel.c`, and `milestone1/Makefile`. This folder is the intended home for the next deliberate migration. Do not duplicate the loader here; move it only when the root build and QEMU tests are updated in the same change.
