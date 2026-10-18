# Component catalog

CustomOS currently has 100 component packages in `components/`, grouped into ten domains: boot, kernel, memory, process/task, storage, drivers, terminal, XC language, UI, and security.

Each component has five tracked files:

1. `include/module.h` — stable public C interface.
2. `src/module.c` — minimal independently compilable implementation.
3. `tests/test_module.c` — executable API/self-check test.
4. `module.json` — machine-readable component metadata and maturity flag.
5. `README.md` — responsibility, API, and maturity notes.

The 500 files are real source, interface, test, manifest, and documentation files, not empty placeholders. They are deliberately **catalog scaffolds**, not claims of 100 finished OS features. The API currently provides stable component identity, a unique catalog ID, initialization state, and a self-check. CI verifies every manifest, checks for unique IDs, compiles each C component with warnings treated as errors, and executes each self-test.

Run:

```sh
python3 scripts/test_component_catalog.py
```

The boot ISO does not yet link these components into its kernel or userspace. Integration is the next engineering phase: replace scaffolding behavior with domain logic, add platform-specific interfaces, and only then wire a component into the build.
