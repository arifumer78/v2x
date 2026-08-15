# runtime/

Phase 1 — the hand-built, fuzz-tested X.691 PER codec kernel. UPER (Unaligned,
Basic) is implemented and unit-tested. Built on top of it: a generic,
hand-usable orchestration layer for the ASN.1 basic type constructs
(SEQUENCE, CHOICE, SEQUENCE OF, ENUMERATED, OCTET STRING, BIT STRING) —
not tied to any specific ETSI `.asn` module, and directly reusable by the
eventual compiler's codegen. See [`../STATUS.md`](../STATUS.md) for the
honest checklist.

Design: see [`../docs/design/per-runtime-design.md`](../docs/design/per-runtime-design.md).

## Building and testing

```
cmake -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

## Running under AddressSanitizer

MSVC supports ASan (`/fsanitize=address`) directly. A separate build
directory is used since ASan needs a Release-family config (`/RTC`, MSVC's
default Debug runtime checks, is incompatible with `/fsanitize=address`):

```
cmake -B build-asan -G "Visual Studio 18 2026" -A x64 -DV2X_ENABLE_ASAN=ON
cmake --build build-asan --config RelWithDebInfo
```

The ASan runtime DLL (`clang_rt.asan_dynamic-x86_64.dll`) ships with the MSVC
toolset but usually isn't on `PATH` — without it the tests fail immediately
with exit code `0xc0000135` (`STATUS_DLL_NOT_FOUND`), not an ASan finding.
Add its directory before running `ctest`:

```
$env:PATH = "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\<version>\bin\Hostx64\x64;" + $env:PATH
ctest --test-dir build-asan -C RelWithDebInfo --output-on-failure
```

(Substitute the actual MSVC toolset version under `VC\Tools\MSVC\`.)

## Running under UndefinedBehaviorSanitizer

Plain MSVC (`cl.exe`) doesn't support UBSan — it's clang/GCC-only, needing
the "C++ Clang Compiler for Windows" VS component (installs `clang-cl.exe`,
distinct from `clang-tidy`/`clang-format`, which are a separate, smaller
component and don't include this). With that installed, build using the
`ClangCL` platform toolset instead of plain MSVC:

```
cmake -B build-ubsan -G "Visual Studio 18 2026" -A x64 -T ClangCL -DCMAKE_BUILD_TYPE=RelWithDebInfo -DV2X_ENABLE_UBSAN=ON
cmake --build build-ubsan --config RelWithDebInfo
ctest --test-dir build-ubsan -C RelWithDebInfo --output-on-failure
```

No `PATH`/DLL setup needed here (unlike ASan above) — `clang-cl` statically
links its UBSan runtime by default. That static runtime lib is only built
for the static CRT, though, which is why `V2X_ENABLE_UBSAN` forces
`CMAKE_MSVC_RUNTIME_LIBRARY` to static for the whole build (GTest included)
rather than leaving CMake's usual dynamic-CRT default — see the comments in
`CMakeLists.txt` if this ever needs revisiting. `-DCMAKE_BUILD_TYPE` must be
set explicitly at configure time here (the `ClangCL` toolset doesn't apply
CMake's usual "falls back to Release with a warning" default the way plain
MSVC does).

## Running the fuzz harnesses (Layer 5)

Needs the `ClangCL` toolset, same as UBSan — but its own build directory, not
`build-ubsan`: `clang_rt.fuzzer-x86_64.lib` is static-CRT-only like UBSan's
runtime, but this LLVM distribution's ASan main lib is dynamic-CRT-only, so
fuzzing links the dynamic ASan lib alongside a static-CRT executable via
clang-cl's `asan_static_runtime_thunk` shim — a different, incompatible
combination from UBSan's own static-CRT UBSan runtime. `V2X_ENABLE_FUZZ` and
`V2X_ENABLE_UBSAN` refuse to be combined in one configure (see the CMake
error if you try) for exactly this reason.

```
cmake -B build-fuzz -G "Visual Studio 18 2026" -A x64 -T ClangCL -DCMAKE_BUILD_TYPE=RelWithDebInfo -DV2X_ENABLE_FUZZ=ON
cmake --build build-fuzz --config RelWithDebInfo --target fuzz_primitives fuzz_strings_and_sequence fuzz_choice_enum_extension
```

Each is a standalone libFuzzer executable (not a CTest target — libFuzzer
supplies its own `main`). The ASan DLL needs to be next to the `.exe` (or on
`PATH`), same requirement as plain ASan above:

```
Copy-Item "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\lib\clang\<ver>\lib\windows\clang_rt.asan_dynamic-x86_64.dll" build-fuzz\runtime\tests\fuzz\RelWithDebInfo\
.\build-fuzz\runtime\tests\fuzz\RelWithDebInfo\fuzz_primitives.exe -max_total_time=60
```

(Substitute the actual LLVM version under `lib\clang\<ver>\`.) Drop
`-max_total_time` to run indefinitely; each harness prints its own
recommended dictionary and crash count on exit.

## Test coverage status

GTest Layer 1 (known-answer, boundary, fragmentation, zero-length) is done —
163 tests. Layer 2 (property-based round-trip) and Layer 3 (double round-trip)
are also done — 31 more tests in `tests/property/` (a separate CTest target,
`v2x_per_property_tests`; picked up automatically by the same `ctest` sweep
below). 194 tests total, all passing, verified clean under both ASan and UBSan.

Layer 2/3 use hand-rolled random generators (`tests/property/random_gen.hpp`),
not an external property-testing library — a fixed default seed, overridable
via the `V2X_PROPERTY_SEED` environment variable and printed at the start of
every run, makes a failure reproducible by rerunning with that seed (no
shrinking, unlike a real framework — not needed yet at this project's scale).

Layer 5 (libFuzzer+ASan) has three harnesses under `tests/fuzz/` — see above
to build/run them. Differential-vs-`asn1c` (Layer 4) is on hold: it needs a
WSL distro installed (WSL2 itself is active on this machine, but zero distros
are currently installed) plus `gcc`/`make`/`bison`/`flex` and `asn1c` built
from source — real setup work, deliberately deferred for now.
