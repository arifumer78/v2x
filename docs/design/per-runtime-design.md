# Design Document: X.691 PER Runtime Kernel

**Status:** Draft v0.1
**Scope:** Phase 1 only — the hand-built, fuzz-tested runtime. ASN.1 compiler (ANTLR-based front end, IR, codegen) is a separate, later design document and explicitly out of scope here.

---

## 1. Purpose and Scope

This document specifies a standalone C++ runtime library implementing **ITU-T X.691 Packed Encoding Rules (PER)** — both **Aligned (APER)** and **Unaligned (UPER)** variants, with **Canonical (CPER/CUPER)** as an orthogonal extension — sufficient to encode and decode ETSI ITS facilities-layer messages (CAM, DENM, CPM, VAM, MAPEM/SPATEM/IVIM/SREM/SSEM/RTCMEM) once a compiler is built on top of it.

The runtime is the trusted kernel of the whole project. It must be correct, fuzz-hardened, and safety-conscious *before* any code generation exists, so that once a compiler exists, every bug it might produce is a codegen bug, not a bit-packing bug.

### 1.1 Non-Goals (explicitly out of scope for this document)

- **BER, DER, XER, JER** — not needed; ETSI ITS mandates PER exclusively.
- **COER (X.696)** — required for IEEE 1609.2 / ETSI TS 103 097 security structures, but is a *different* encoding with different primitives (octet-aligned, canonical-by-default, open-type wrapping rules distinct from PER's). Treated as a future, separate codec (see §8).
- **Non-PER-visible constraints** — pattern constraints, user-defined constraints, contained-subtype constraints that don't affect the wire encoding. These are semantic/validation concerns, not encoding concerns, and the ETSI ITS message set does not appear to use them in any way that affects the wire format.
- **The ASN.1 compiler itself** (parser, IR, codegen) — separate effort, separate document, may use C++23 freely since it is a host-side build tool, not a shipped artifact.

---

## 2. Design Principles

1. **Reduction to primitives.** X.691 defines a small number of encoding procedures (§10 of the spec); every ASN.1 construct is expressed as orchestration over these, never as bespoke bit-packing logic per type. This is the core architectural bet of the whole project.
2. **Runtime correctness precedes tooling.** No compiler work begins until this kernel is unit-tested, property-tested, differentially validated against `asn1c`, and fuzz-hardened.
3. **Safety-conscious by default.** The runtime is written as if it will run on an ECU, even though initial use is host-side. No exceptions, no dynamic allocation, no RTTI, bounded execution — see §7.
4. **Alignment and canonicality are policies, not forks.** APER/UPER and Basic/Canonical are orthogonal compile-time policies over one shared primitive implementation, not separate codebases.
5. **Security (1609.2/TS 103 097) is explicitly deferred.** COER is a different encoding; conflating it with this PER kernel would blur the design. The facilities-layer payload is decoded by this runtime; the signed security envelope is treated as an opaque blob for now.

---

## 3. Standards Basis

| Concern | Reference |
|---|---|
| Core PER encoding procedures | ITU-T X.691 clause 10 |
| Constraint semantics (PER-visible vs not) | ITU-T X.680/X.682 |
| ETSI mandated encoding | Unaligned PER exclusively for over-the-air ETSI ITS messages |
| Security structures (deferred) | IEEE 1609.2, ETSI TS 103 097 — encoded in COER (X.696), not PER |

---

## 4. Primitive API

### 4.1 The Seven Primitives

Per X.691 §10, all constructs reduce to these operations. Both `PerWriter` and `PerReader` expose the symmetric pair for each:

| Primitive | X.691 clause | Used for |
|---|---|---|
| `constrained_whole_number` | §10.5 | Ranged INTEGER, CHOICE index, ENUMERATED index, SIZE-constrained length within known bounds |
| `semi_constrained_whole_number` | §10.7 | Lower-bound-only INTEGER |
| `unconstrained_whole_number` | §10.8 | Unbounded INTEGER (two's complement, length-prefixed) |
| `normally_small_length` | §10.6 | Extension-addition group counts, small lengths |
| `length_determinant` | §10.9 | General-case length, including SIZE-constrained and the 16K fragmentation path |
| `bits` | — | Raw bit-blit for OCTET STRING / BIT STRING content once length is known |
| `bit` | — | BOOLEAN, presence/extension flags |
| `open_type` | extension rules | Extension-addition wrapping, unknown-CHOICE-alternative fallback |

### 4.2 Interface Sketch

```cpp
template <typename AlignPolicy, typename CanonicalPolicy = Basic>
class PerWriter {
public:
    explicit PerWriter(std::span<std::byte> out_buffer);

    std::expected<void, Error> constrained_whole_number(int64_t v, int64_t lb, int64_t ub);
    std::expected<void, Error> semi_constrained_whole_number(int64_t v, int64_t lb);
    std::expected<void, Error> unconstrained_whole_number(int64_t v);
    std::expected<void, Error> normally_small_length(uint32_t n);
    std::expected<void, Error> length_determinant(uint32_t n,
                                                    std::optional<SizeRange> constraint);
    std::expected<void, Error> bits(std::span<const std::byte> raw, size_t n_bits);
    std::expected<void, Error> bit(bool);
    std::expected<void, Error> open_type(std::span<const std::byte> encoded);

    size_t bits_written() const;
    std::span<const std::byte> finish();   // flushes padding per CanonicalPolicy
};

template <typename AlignPolicy, typename CanonicalPolicy = Basic>
class PerReader {
public:
    explicit PerReader(std::span<const std::byte> in_buffer);

    std::expected<int64_t, Error> constrained_whole_number(int64_t lb, int64_t ub);
    std::expected<int64_t, Error> semi_constrained_whole_number(int64_t lb);
    std::expected<int64_t, Error> unconstrained_whole_number();
    std::expected<uint32_t, Error> normally_small_length();
    std::expected<uint32_t, Error> length_determinant(std::optional<SizeRange> constraint);
    std::expected<void, Error> bits(std::span<std::byte> out, size_t n_bits);
    std::expected<bool, Error> bit();
    std::expected<std::span<const std::byte>, Error> open_type();

    size_t bits_remaining() const;
};
```

### 4.3 Orchestration (built on the primitives, not part of the kernel API surface — lives in compiler-generated code, documented here for completeness)

- **SEQUENCE**: extension bit (if extensible) → preamble bitmap (one bit per OPTIONAL/DEFAULT field) → each present field via its constraint-appropriate primitive → if extension bit set: `normally_small_length` for addition-group bitmap, each addition individually optional-bit + `open_type`-wrapped.
- **CHOICE**: extension bit → `constrained_whole_number(index, 0, n_alts-1)` for root alternatives; `normally_small_length` + `open_type` for extension alternatives.
- **SEQUENCE OF / SET OF**: `length_determinant` (constrained or general, including 16K-chunk fragmentation) → per-element encode. **SET OF under `Canonical` policy** additionally requires encoding each element into a scratch buffer, sorting lexicographically by encoded octets, then concatenating — see §6.2.
- **ENUMERATED**: `constrained_whole_number` over root values; extension values via the CHOICE-style extension path.

---

## 5. Policy Design

### 5.1 Alignment: `Aligned` / `Unaligned`

Same primitive set; alignment affects only **where octet-padding is inserted** (before length determinants not part of a fixed preamble, before OCTET/BIT STRING content above threshold, before open-type content). Implemented as a compile-time policy with an `align()` hook called from specific primitive call sites — never a runtime branch, never a duplicated codec.

```cpp
struct Unaligned { static void align(BitCursor&) {} };
struct Aligned   { static void align(BitCursor& c) { c.pad_to_octet(); } };
```

### 5.2 Canonicality: `Basic` / `Canonical`

Canonical PER adds determinism constraints on top of basic PER, not new bit-packing rules for scalars:

- SET component ordering by canonical tag order (not declaration order).
- SET OF element ordering by encoded-octet comparison (requires the buffer-and-sort orchestration noted in §4.3).
- Trailing/reserved padding bits forced to zero, never left unspecified.

**Scoping note (partially verified 2026-08-08):** grepped the real CAM (TS 103 900) and CDD (TS 102 894-2) `.asn` modules — see `compiler/tests/fixtures/` — for `SET`/`SET OF` type definitions: **zero occurrences in either file.** Every aggregate type in both modules is `SEQUENCE`/`SEQUENCE OF`. For these two modules, Basic and Canonical PER produce **byte-identical output**, and the SET-ordering/sort machinery is confirmed dead code even though it remains implemented for completeness. Not yet checked: DENM, CPM, VAM, MAPEM/SPATEM/IVIM/SREM-SSEM/RTCMEM — repeat this grep once those modules are pulled in (§9).

### 5.3 Combined Instantiations

```cpp
using UperWriter  = PerWriter<Unaligned, Basic>;
using AperWriter  = PerWriter<Aligned,   Basic>;
using CuperWriter = PerWriter<Unaligned, Canonical>;
using CperWriter  = PerWriter<Aligned,   Canonical>;
```

`UperWriter`/`UperReader` are the primary target (ETSI ITS mandate); the others exist for generality and future reuse, at effectively zero extra maintenance cost given the shared kernel.

---

## 6. Constraint Model

### 6.1 In Scope (PER-visible — affects wire encoding)

| Constraint | Effect on encoding |
|---|---|
| Single-value / value-range (INTEGER) | Determines constrained vs. semi-constrained vs. unconstrained primitive, and bit-width |
| SIZE | Determines length-determinant behavior (fixed, ranged, or general) |
| Extensibility marker (`...`) | Root vs. extension-addition encoding path — applies to INTEGER, ENUMERATED, SEQUENCE, CHOICE, SIZE |
| DEFAULT (on SEQUENCE components) | Treated as OPTIONAL for PER purposes — presence bit governs whether encoded; default value itself is a semantic, not wire, concern |
| PermittedAlphabet | Rare in ITS modules; affects character-string encoding width |

### 6.2 Out of Scope (non-PER-visible — validation only, not encoding)

Pattern constraints, user-defined constraints, and most contained-subtype cases do not affect PER encoding per X.691 and are not represented in this runtime's constraint model at all. If the future compiler's IR encounters one, it should be parsed and ignored for encoding purposes (optionally surfaced as a hook for application-layer validation, never passed to `PerWriter`/`PerReader`).

### 6.3 Constraint Algebra

Effective constraints (after intersection/inheritance across type references) are assumed to be **pre-resolved by the caller** (i.e., the future compiler's IR). The runtime primitives take a single, already-resolved bound/range — the runtime performs no constraint algebra itself. This keeps the kernel's responsibility strictly to "given resolved bounds, pack/unpack bits correctly."

---

## 7. Safety and Compliance Posture

**Target:** MISRA C++:2023 / AUTOSAR-C++-informed discipline for this runtime specifically, even though the toolchain used to build it may be C++23. Rationale: MISRA C++:2023 currently targets C++17 semantics only; no published MISRA guidance exists yet for C++20/23. The runtime is written to a conservative subset so it remains meaningfully auditable against current tooling, while the separate compiler (host-side only, never shipped to a target) is free to use full C++23.

Concrete rules applied to this runtime:

- **No exceptions.** All fallible operations return `std::expected<T, Error>`.
- **No dynamic allocation on the hot path.** `std::span` over caller-provided or statically-sized buffers; buffer sizing intended to be derivable at compile time from a type's maximum encoded size once the compiler exists.
- **No RTTI, no `dynamic_cast`.** Not needed; no polymorphic type hierarchy in the kernel.
- **Bounded execution.** No unbounded loops driven directly by untrusted input length without an explicit sanity ceiling (defends against malformed length-determinants causing runaway iteration — dual-purpose: safety discipline and fuzz-resilience).
- **No implicit narrowing conversions.** Explicit casts only; UPER's exact bit-width packing makes silent truncation a realistic bug class.
- **No unconditional recursion.** Orchestration-level recursion (for nested SEQUENCE/CHOICE, once codegen exists) must be provably bounded; self-referential ASN.1 types are a known open risk (§9).

**Explicitly deferred:** formal MISRA tooling (Cppcheck, Coverity, Parasoft, etc.) is not required for this phase. The above is treated as engineering discipline, not a certification claim, unless/until a concrminate future need for formal compliance emerges.

---

## 8. Boundary with Security Structures (1609.2 / TS 103 097)

Explicitly **out of scope** for this runtime. Rationale:

- 1609.2 structures are encoded in **COER (X.696)**, not PER — a different primitive set (octet-aligned, canonical-oriented), not a policy variant of this kernel.
- Heavy use of algorithm-agility CHOICE-of-CHOICE-of-open-type patterns (`PublicVerificationKey`, `HashAlgorithm`, `SignerIdentifier`, etc.) that would significantly complicate this kernel's scope if merged in now.
- No structural recursion identified in 1609.2 (certificate chaining is by `HashedId8` reference, not embedded recursive structures), so it does not currently pose the same "unbounded recursive type" risk as a hypothetical recursive facilities-layer type — but it remains a separate, sizeable effort.

**Interim strategy:** the security envelope is treated as an opaque signed blob at the facilities-layer boundary — verified/stripped by an existing library or passed through unexamined — with this runtime only ever decoding the payload inside. A `CoerWriter`/`CoerReader` sharing the *design pattern* (not the code) of this kernel is a candidate Phase 4 effort.

---

## 9. Known Open Risks

| Risk | Status |
|---|---|
| Self-referential / recursive ASN.1 types | **Attempted a text-based check 2026-08-08, found unreliable — not resolved.** A regex heuristic over the real CAM/CDD `.asn` text flagged ~20 apparent "self-references," but spot-checking (`StationId`) showed these are false positives from ASN.1-style doc comments (e.g. `@ref StationId` elsewhere in the file), not actual recursive field embedding — naive text scoping can't reliably separate one type's definition body from surrounding doc comments and later definitions. A real check needs to walk the validated parse tree (`compiler/grammar/`) and build the type-reference graph properly; not yet done. Compiler-level IR should detect cycles and reject unless a static depth bound is provided — never silently allocate. Runtime itself does not need to solve this; it's a compiler-level constraint-checking concern, noted here for traceability. |
| Unbounded SIZE constraints | **Confirmed absent in CAM/CDD, 2026-08-08.** Every `SIZE(...)` constraint in both real modules has a concrete, literal upper bound (e.g. `SIZE(1..16,...)`) — zero occurrences of the `MAX` keyword in either file. Not yet checked for DENM/CPM/VAM/etc. Should still be a compiler-level hard error ("reject unless provably bounded") for any module where it does occur, not a runtime surprise. |
| SET / SET OF usage in ETSI ITS modules | **Confirmed absent in CAM/CDD, 2026-08-08** — see §5.2. DENM/CPM/VAM/MAPEM-etc. not yet checked; repeat the grep once those modules are pulled into `compiler/tests/fixtures/`. |
| 16K fragmentation path | Plausible to trigger in CPM's `PerceivedObjectContainer` on a busy intersection. Must not be skipped in either implementation or test coverage. |

---

## 10. Test Plan Summary

(Full detail in separate test plan; summarized here for design-document completeness.)

1. **GTest unit tests** per primitive, parameterized across policy combinations — known-answer (X.691 worked examples), boundary values, fragmentation boundary (16383/16384/16385), zero-length cases.
2. **Property-based round-trip tests** (RapidCheck or similar) — random valid values per constraint, `decode(encode(v)) == v`.
3. **Double round-trip tests** — `encode→decode→encode→decode`, asserting value equality, **byte-identical re-encoding**, and transitivity. This layer is the primary defense against non-deterministic/non-canonical encoding bugs.
4. **Differential testing against `asn1c`** — same modules through both toolchains; byte-diff encoder output, field-diff decoder output against real captured UPER messages.
5. **Fuzzing** (libFuzzer/AFL++, ASan+UBSan) — reader-focused, one harness per primitive plus whole-message once codegen exists; seeded from known-answer tests and real captures; explicit coverage of malformed extension bits and open-type content.
6. **Whole-message integration tests** — deferred until compiler exists (Phase 2), included here for continuity of the test taxonomy.

**Acceptance criterion for declaring this runtime "done" for Phase 1 purposes:** all layers 1–4 passing, layer 5 running with a stable/plateaued coverage corpus over a sustained nightly run, 100% branch coverage target on `PerReader`/`PerWriter` given the kernel's small size.

---

## 11. Explicit Deferrals (tracked, not forgotten)

- ASN.1 compiler front end (ANTLR grammar, IR, codegen) — see [`compiler-frontend-design.md`](compiler-frontend-design.md).
- COER/X.696 codec for 1609.2/TS 103 097 — Phase 4 candidate.
- Formal MISRA/AUTOSAR tooling and certification claims — only if a concrete need emerges.
- SET/SET OF canonical sort path validation — implement for completeness, defer rigorous testing until confirmed relevant to in-scope modules.

---

*This is a living draft. Sections 4 (API) and 9 (risks) are expected to change first as implementation begins.*
