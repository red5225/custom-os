# XC language implementation

This directory is the home for the XC language itself—not example applications. Compiler, parser, type system, module system, standard library, and runtime work belongs here or in clearly named `xc-` directories.

## Current status

The existing host interpreter and starter code generator are prototypes with a small statement grammar. They do not yet support user-defined functions, structs, modules/imports, loops, generics, a type checker, or native code generation independent of C. Those are implementation tasks, not assumed capabilities.

## Required compiler pipeline

1. Lex source into tokens with source spans.
2. Parse tokens into an AST; reject malformed syntax with file/line/column diagnostics.
3. Resolve modules/imports and names.
4. Type-check and validate effects/permissions for privileged APIs.
5. Lower XCUI-generated declarations and normal XC code into a shared typed IR.
6. Emit C as the bootstrap backend; later add a native backend without changing XC semantics.
7. Compile/link and report errors against original XC source locations.

## Language contract

See `LANGUAGE_SPEC.md` for the planned grammar and required semantics. This is a specification target; each feature must be marked implemented only after compiler tests pass.