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

MSVC supports ASan (`/fsanitize=address`) but not UBSan — UBSan is
clang/GCC-only and would need the "C++ Clang tools for Windows" (`clang-cl`)
component, not currently installed. A separate build directory is used since
ASan needs a Release-family config (`/RTC`, MSVC's default Debug runtime
checks, is incompatible with `/fsanitize=address`):

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

## Test coverage status

GTest Layer 1 (known-answer, boundary, fragmentation, zero-length) is done —
151 tests, all passing, also verified clean under ASan. Property-based
round-trip, double round-trip, differential-vs-asn1c, and fuzzing (Layers
2–5) are not started yet.
