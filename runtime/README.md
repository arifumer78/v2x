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

## Test coverage status

GTest Layer 1 (known-answer, boundary, fragmentation, zero-length) is done —
163 tests, all passing, verified clean under both ASan and UBSan. Property-based
round-trip, double round-trip, differential-vs-asn1c, and fuzzing (Layers
2–5) are not started yet.
