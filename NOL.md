# XCL — eXecutable Core Language

XCL is the main source language of CustomOS.

Most OS-facing behavior is stored in .xcl files under src.

The current bootstrap compiler is tools/xclc.c. There is no Python compiler or Python build dependency.

Long term: XCL source -> lexer/parser -> XCL IR -> native x86.

Roadmap: variables, integers, expressions, loops, structs, memory operations, modules, native x86 backend, standard library, and a self-hosting XCL compiler.
