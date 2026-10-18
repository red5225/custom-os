# XC — eXecutable Custom

XC is a small, readable language for CustomOS. The initial interpreter is implemented in Python 3 and uses a deliberately small command set. It is a first milestone, not yet a native compiler.

## Example

```xc
program "Hello CustomOS"
say "Welcome!"
let score = 10
add score 5
show score
ask name
if name == "Nolan"
  say "Welcome back!"
else
  say "Hello!"
end
finish
```

## Statements

- `program "Title"` — optional program title (documentation/identity only).
- `say "text"` — print text; a variable name or number can also be printed.
- `let name = value` — create or replace a variable. Values can be quoted strings, numbers, or existing variables.
- `add name number` — add a numeric amount to an existing numeric variable.
- `show name` — print a variable.
- `ask name` — read one line from the user into a variable.
- `if name == value`, optional `else`, and `end` — compare a variable with a value.
- `finish` — stop the program.
- Lines beginning with `#` are comments.

XC source files must end in `.xc`. Invalid syntax and unknown commands produce a line-numbered error. XC does not run shell commands, eval, or arbitrary Python code from a program file. This helps keep early scripts predictable.

## Run

```sh
xc apps/example/hello.xc
customos xc apps/example/hello.xc
```

## Implementation plan

1. Current: interpreter, variables, basic arithmetic, input, conditionals, tests.
2. Next: loops, functions, lists, files, and clearer diagnostics.
3. Later: native XC bytecode/VM and system APIs, with explicit permissions for filesystem, network and device access.
