# XC language specification (initial host interpreter)

XC is a small, readable command language. The current implementation is `tools/xc/xc.cpp`; it runs on the host and is not yet embedded in the kernel.

## Statements

- `program "name"` declares a program name.
- `say "text"` writes a line.
- `let identifier = value` defines a string or integer variable.
- `add identifier integer` adds an integer to an integer variable.
- `show identifier` prints a variable.
- `ask identifier` reads one line from standard input.
- `if identifier == value`, `else`, `end` select a branch.
- `finish` ends the program.
- Lines beginning with `#` and blank lines are ignored.

## Values and names

Quoted strings support basic backslash escapes. Integer literals are base-10 signed integers. Identifiers start with an ASCII letter and may then contain letters, digits, or underscores. Equality currently compares value text, so integer `42` and string `"42"` compare equal; strict type-aware equality is planned.

## Current limits

No functions, loops, arrays, imports, modules, file I/O, exceptions, or kernel execution are implemented yet. Unsupported statements should fail with a line-numbered error instead of silently running. Nested-condition and malformed-input coverage still needs to grow.
