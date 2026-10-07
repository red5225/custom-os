# NOL — Native OS Language

NOL is the main source language of CustomOS.

## Current structure

Most OS-facing behavior is stored in .nol files under src:

- src/boot.nol — boot banner
- src/shell.nol — command dispatcher
- src/help.nol — user-facing commands
- src/system.nol — status and credits
- src/terminal.nol — terminal personality

The remaining C and assembly is the small hardware boundary: boot entry, VGA terminal primitive, PS/2 keyboard input, and the bootstrap runtime.

## Build model

There is no Python compiler or Python build dependency.

A tiny C bootstrap compiler, tools/nolc.c, translates the current NOL subset into freestanding C during the host build. This is temporary compiler infrastructure, not the OS language.

The intended evolution is:

NOL source -> NOL lexer/parser -> NOL IR -> native x86 code

That lets the bootstrap C compiler eventually disappear too.

## Current NOL subset

- functions
- string output
- command matching
- function calls
- terminal clear/backspace operations
- comments

## Next language work

1. lexer and parser
2. AST and NOL IR
3. integers and variables
4. loops and richer expressions
5. memory and pointers
6. native x86 backend
7. NOL standard library
8. move more kernel services from C into NOL
