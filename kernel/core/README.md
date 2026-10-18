# Kernel core primitives

These small freestanding-friendly C functions provide tested building blocks for later kernel integration. Each `.c` file has a matching public header and a host unit test under `tests/core/`. The current CI compiles every module/test pair with `-Wall -Wextra -Werror`.

**Integration status:** the current booted kernel entry remains `kernel/kernel.c`. The core modules are tested as a library but are not yet linked into the freestanding kernel image; do not treat them as active drivers or claim hardware functionality based on these unit tests.
