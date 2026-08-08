# Changelog

Dated, terse entries — enough to answer "what changed since I last looked," not a full commit log.

## 2026-08-08 (V2V communication overview image)
- Added `docs/images/v2v-communication-overview.png`

## 2026-08-08 (hitchhiker's guide ecosystem section)
- Added a "Related ecosystem — AD stacks & cooperative perception" subsection to `docs/hitchhikers-guide.md` §6, grouped by layer: AD stack integration (Autoware, Baidu Apollo, AutowareV2X, AutoC2X/OpenC2X), cooperative perception/fusion (OpenCOOD, V2X-ViT, CoBEVT, plus the DAIR-V2X/V2X-Real/V2X-Sim/OPV2V datasets they train against), and this repo's own comms/message layer for contrast. Framing: fusion pipelines still have to serialise/deserialise standardised messages at their edges, so message-layer work stays distinct, underserved connective tissue rather than something the field has moved past.
- Bumped the guide's "Last reviewed" date accordingly.

## 2026-07-19 (full clause 12–23 cross-check + constraint-level extensibility)
- Read every X.691 construct-encoding clause the orchestration layer touches (§12–§14, §16–§23, plus §11.2) directly, rather than relying on the design doc's §4.3 summary of them. Confirmed BOOLEAN, INTEGER, ENUMERATED, CHOICE, NULL, `open_type`, and the general/fixed-length forms of BIT STRING/OCTET STRING/SEQUENCE/SEQUENCE OF all correct — including several spec special cases (zero-length strings, fixed-length strings, single-alternative CHOICE) that turn out to be satisfied automatically by `constrained_whole_number`'s `span==0` case collapsing to zero bits, without any deliberate special-casing on our part. Confirmed SET OF under Basic-PER reuses `sequence_of.hpp` directly (§22.2).
- **Found a real, previously-unimplemented gap: constraint-level extensibility** (e.g. `INTEGER (0..10, ...)`, `OCTET STRING (SIZE(1..10,...))`) — distinct from the structural extensibility (SEQUENCE addition groups, CHOICE alternatives, ENUMERATED values) already built. Implemented for all four constructs that need it: new `runtime/include/v2x/per/integer.hpp` (§13.1), and `write_extensible_*`/`read_extensible_*` additions to `octet_string.hpp` (§17.3), `bit_string.hpp` (§16.6), `sequence_of.hpp` (§20.4). New `runtime/tests/unit/test_extensible_constraints.cpp` covers all four with hand-verified in-root/out-of-root known-answer bit counts plus round-trips.
- Verified `ExtensionAdditionGroup` support (§19.9, nested multi-field addition groups with their own inner preamble) via a new test (`SequenceExtension.ExtensionAdditionGroupNestedPreamble`) rather than a code change — the existing generic callback design in `sequence_extension.hpp` already handles it correctly.
- Deliberately not implementing SEQUENCE preamble/addition-bitmap fragmentation for ≥64K fields (§19.3) — no realistic ASN.1 type needs it.
- 163 tests total (up from 154), all passing under both the Debug build and `-DV2X_ENABLE_ASAN=ON`.

## 2026-07-19 (X.691 spec cross-check)
- Downloaded the official ITU-T X.691 (02/2021) PDF (freely available at itu.int) and used it as ground truth against the existing implementation, rather than relying on hand-derivation or self-consistent tests alone.
- **Found and fixed a real bug in `normally_small_length`** (§11.6): the small-form threshold was `[1,64]` instead of the spec's `n<=63`; the small form encoded `n-1` instead of `n` directly; and the large-form path (`n>=64`) called the general `length_determinant(n)` — a structurally different, wrong encoding — instead of encoding `n` as a semi-constrained whole number. For `n=64` the old code produced 9 bits where the correct encoding is 17. All 154 tests had been passing throughout, since they only checked self-consistency, never the spec — the exact gap this exercise was for. Removed the now-unnecessary `ext_index+1`/`-1` compensation this bug had forced into `choice.hpp` and `enumerated.hpp`.
- Added `runtime/tests/unit/test_x691_spec_examples.cpp`: 4 known-answer tests transcribed directly from the spec (not hand-derived), each citing its source clause/annex — `unconstrained_whole_number(51)` from Annex A.1's PersonnelRecord example, `length_determinant` short/medium form from §11.9.3.6/.7's own worked examples, and `length_determinant` fragmentation verified against §11.9.3.8.1's own 144K+1 (147457-item) example, matching the spec's exact `[65536, 65536, 16384, 1]` chunk sequence and marker bytes.
- 154 tests total (up from 151), all passing under both the Debug build and `-DV2X_ENABLE_ASAN=ON`.

## 2026-07-19 (ASN.1 basic-type orchestration layer)
- Built generic, hand-usable support for the X.691-mandated ASN.1 constructs on top of the Phase 1 primitive kernel — SEQUENCE, CHOICE, SEQUENCE OF, ENUMERATED, OCTET STRING, BIT STRING — deliberately not tied to any specific ETSI `.asn` module and not a reflection-driven generic type system, per explicit direction: build the X.691 encoding rules generically first, ahead of the ANTLR `.asn` parser (Phase 2 proper). New headers in `runtime/include/v2x/per/`: `octet_string.hpp`, `bit_string.hpp`, `sequence_of.hpp`, `open_type_wrap.hpp`, `sequence_extension.hpp`, `enumerated.hpp`, `choice.hpp`.
- Refactored `open_type`'s internal fragmentation loop into a shared free function (`write_length_prefixed_content`/`read_length_prefixed_content`, parameterized by `unit_bits`) so OCTET STRING (octet-counted) and BIT STRING (bit-counted — a real X.691 distinction, not a simplification) can reuse it. Refactor verified behavior-preserving against the full pre-existing test suite before building anything on top of it.
- Found and fixed a real bug during implementation: `write_sequence_extension` initially wrote the SEQUENCE extension bit internally, inconsistent with the planned caller-writes-it usage pattern — caught by a round-trip test decoding one bit off. Fixed by making the extension bit the caller's responsibility on both write and read, symmetrically.
- `read_sequence_extension` handles forward-compatible decoding correctly: addition-group entries beyond this decoder's own known count (a newer sender using spec-later extensions) are consumed but not decoded, not treated as an error — matching real ASN.1 extensibility semantics.
- Added a worked example (`test_worked_example_sequence.cpp`): a hand-written, non-generated SEQUENCE type (2 mandatory INTEGER, 1 optional BOOLEAN, extensible with 1 addition-group INTEGER) round-trips correctly across all presence/absence combinations — the real test of whether the design composes usably, not just correct per-construct.
- 151 GTest cases total (up from 97), all passing under both the normal Debug build and `-DV2X_ENABLE_ASAN=ON`.

## 2026-07-19 (runtime)
- CMake scaffold for `runtime/`: root `CMakeLists.txt` + `runtime/CMakeLists.txt`, Visual Studio 17 2022 generator, GoogleTest via `FetchContent`, CTest wired up (`gtest_discover_tests`).
- Implemented the full UPER (Unaligned, Basic) primitive kernel in `runtime/include/v2x/per/`: `BitCursor`, `Error`, hand-rolled `Result<T,E>`/`Status<E>`, `SizeRange`, and all seven `PerWriter`/`PerReader` primitives (`bit`, `bits`, `constrained_whole_number`, `semi_constrained_whole_number`, `unconstrained_whole_number`, `normally_small_length`, `length_determinant`, `open_type`) — 97 GTest Layer-1 cases (known-answer, boundary, fragmentation, zero-length), all passing.
- Deviated from the design doc's draft API (§4, explicitly flagged there as likely to change): replaced `std::expected` with a project-owned `Result`/`Status` (no published MISRA C++:2023 guidance exists yet for `std::expected`, a C++23 feature); changed `length_determinant` to return the covered chunk size rather than `void`, and `PerReader::open_type` to take a caller-provided destination span — both needed because real X.691 fragmentation interleaves length markers with content, which a content-blind primitive can't do alone. Full rationale in the code comments at each call site.
- `Aligned`/`Basic`/`Canonical` policy structs exist and all four `PerWriter`/`PerReader` policy combinations compile and pass a smoke test, but only `Unaligned`/`Basic` (UPER) is behaviorally complete — see the wiring-status note in `policy.hpp`.
- Safety-posture sweep against design doc §7: no exceptions, no heap allocation, no RTTI, no recursion, and the only two `while(true)` loops (both in `open_type`) are bounded by construction (buffer size / 16 KiB per fragmenting iteration).
- `STATUS.md` updated to reflect all of the above; Layers 2–5 (property-based, double round-trip, differential-vs-asn1c, fuzzing) remain unstarted.
- Added a `V2X_ENABLE_ASAN` CMake option (`runtime/CMakeLists.txt`) and verified the existing 97-test suite clean under AddressSanitizer (separate `build-asan` dir, RelWithDebInfo config — `/RTC` conflicts with `/fsanitize=address` under MSVC's default Debug flags). No memory-safety findings. This is not fuzzing — it only re-runs the existing fixed test inputs under the sanitizer, not random/adversarial ones. UBSan isn't available under plain MSVC (clang/GCC-only); noted as a gap. Build/run instructions, including the ASan-runtime-DLL-on-PATH gotcha, documented in `runtime/README.md`.

## 2026-07-19
- Added `docs/architecture/pc5-vs-uu.md` — PC5 vs Uu interface comparison, topology diagram, and why Uu/V2N backend is comparatively under-standardized.
- Added `docs/messages/README.md` — reference table of all facilities-layer messages (trigger model, interface, spec, ASN.1 module).
- Added `docs/messages/flows.md` — Mermaid sequence diagrams for CAM (periodic, dynamically-triggered) and DENM (event-triggered, lifecycle) flows, with notes on why trigger logic and encoding are kept as separate concerns.
- Confirmed MCM (TS 103 561) is still an ongoing/draft standard as of early 2026 — flagged accordingly in the message reference table rather than treated as equivalent-maturity to CAM/DENM/CPM/VAM.

## 2026-07-04
- Restructured repo: moved guide into `docs/`, added `docs/design/`, `runtime/`, `compiler/`, `tools/` scaffolding.
- Added `docs/design/per-runtime-design.md` — Phase 1 design draft for the X.691 PER codec kernel (aligned/unaligned + basic/canonical policies, PER-visible constraint model, no-exceptions/MISRA-informed posture, COER/1609.2 explicitly deferred).
- Added `STATUS.md` to track actual build progress against the phased plan.
- No runtime code yet — design phase only.

## 2026-07-04 (earlier)
- Initial import: `README.md` — Hitchhiker's Guide to ETSI C-V2X (architecture, working groups, active projects, value-add opportunities, core ASN.1 repo).
