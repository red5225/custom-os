# CustomOS — Magneto Edition

CustomOS is an operating-system development project targeting the Acer Chromebook CB314-3H family (x86-64, Intel Celeron N4500). `main` is the integration branch.

## What is real today

- `kernel/`, `Makefile`, `linker.ld`, and `limine.conf` build a freestanding x86-64 kernel using C and a Limine boot path. Its shell is generated from the small `.xcl` bootstrap language.
- `milestone1/` is a separate experimental native UEFI loader/kernel path. It is not yet the default release build.
- `buildroot-external/` and `scripts/build.sh` build the existing Linux/Buildroot-based image. That image is a separate experiment and **is not a fully from-scratch kernel**.
- `buildroot-external/board/customos/overlay/usr/bin/xc` is the XC script interpreter. `.xc` scripts use the language documented in `docs/XC-LANGUAGE.md`. The `.xcl` bootstrap files under `src/` are a distinct legacy kernel-build DSL; do not confuse them with `.xc`. A future migration should unify these grammars before renaming them.
- `tools/host/` contains optional C++ and C# developer utilities. They run on the build host and are not part of the freestanding kernel.

## Language policy

C is used for freestanding kernel and hardware boundaries. C++ is used for host-side developer utilities where its standard library is useful. C# is used for host-side validation tools; it is not linked into the kernel. XC is the custom scripting language. Avoid parallel NOL/XCL language generations: the unused NOL compiler, runtime, docs, and `.nol` examples have been removed.

## Build and test

The default GitHub Actions workflow runs Python host tests, compiles/tests the C++ utility, builds/tests the C# utility, and on `main` builds the Linux/Buildroot ISO with a QEMU serial smoke test. The freestanding Limine kernel has a separate Makefile path (`make`, `make iso`) and downloads its boot protocol/header dependencies at build time.

Download the `customos-linux-x86_64` artifact only from a completed successful Actions run. A QEMU smoke test does not prove that Wi-Fi, audio, touchpad, suspend, internal storage, or Chromebook firmware support works on physical hardware. No physical Chromebook compatibility is claimed without a device test.

Run host tests locally:

```sh
python3 -m unittest discover -s tests -v
g++ -std=c++17 -Wall -Wextra -Werror tools/host/os_inspect.cpp -o /tmp/customos-inspect
/tmp/customos-inspect --self-test
dotnet run --project tools/host/ManifestCheck/ManifestCheck.csproj -- --self-test
```

See [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md), [`docs/CHROMEBOOK-COMPATIBILITY.md`](docs/CHROMEBOOK-COMPATIBILITY.md), and [`docs/BRANCH-MAP.md`](docs/BRANCH-MAP.md).
