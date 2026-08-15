# Design Document: ASN.1 Compiler Front End (Phase 2)

**Status:** Draft v0.1 — grammar/parser layer built and real-file-validated; IR, semantic analysis, and codegen not started.
**Scope:** the ANTLR-based `.asn` parser and its immediate toolchain. IR design, semantic analysis (import resolution, AUTOMATIC TAGS computation, constraint resolution), and codegen targeting `runtime/` are noted here as open work but are a separate, later design pass — this document will grow into that once started, not be replaced by a new one.

Companion document: [`per-runtime-design.md`](per-runtime-design.md) (Phase 1, the PER codec kernel this compiler's codegen will eventually emit calls into).

---

## 1. Purpose and Scope

This document covers `compiler/`: a host-side tool that parses real ETSI ITS `.asn` modules (CAM, DENM, CPM, VAM, and the CDD they all import from) and will eventually generate C++ calling into `runtime/`'s existing PER primitives — per `per-runtime-design.md` §4.3, the orchestration layer already built there is "effectively Phase 2 minus the parser," built and tested ahead of the parser per earlier explicit direction.

This tool is **never shipped to a target ECU**. It runs at build time (developer machine or CI) to turn `.asn` source into `.cpp`/`.h` files that get compiled into the vehicle binary alongside `runtime/`. This distinction matters throughout §7.

---

## 2. Design Principles

1. **Validate against real files, not hand-derived snippets.** Every claim about grammar correctness in this project is backed by parsing actual ETSI-published `.asn` modules pulled from `forge.etsi.org`, not synthetic test input — the same discipline `per-runtime-design.md` §2 applies to spec cross-checking (X.691 PDF as ground truth, not the design doc's own summary of it).
2. **Fork and fix over reinvent.** ASN.1's full grammar (X.680/X.681/X.682/X.683) is large; a working, community-maintained base (`antlr/grammars-v4`'s `asn_3gpp`) is a better starting point than a from-scratch grammar, provided its gaps against real ETSI text are found and fixed rather than assumed away.
3. **Generated code is committed, not synthesized at build time.** Keeps `compiler/`'s own build dependency-free (a C++ toolchain only); regenerating after grammar edits is a documented manual step (`compiler/grammar/README.md`).
4. **The front end's own runtime posture is separate from the generated/deployed code's posture.** See §7 — this is a deliberate, reasoned position, not an oversight.

---

## 3. Grammar and Toolchain

### 3.1 Base grammar: `antlr/grammars-v4`'s `asn_3gpp`, forked

Two candidates from `grammars-v4` were tried against the real CAM module before committing to one:

- `asn/asn` — a 2007-era, ANTLR3-ported grammar. Rejected: cannot lex `/* */` block comments at all, and cannot parse `Type (WITH COMPONENTS {...})` — the second real construct encountered in CAM.
- `asn/asn_3gpp` — modern, 3GPP-flavored, split lexer/parser, has an officially maintained C++ target folder (a real, tested lexer base class) unlike the Python target, which required hand-patching a Java-syntax artifact the codegen didn't translate. Chosen as the base.

**Forked, not used as-is**, once modification began — see `compiler/grammar/AsnEtsiItsParser.g4`'s header comment for the authoritative, current list of changes from upstream. As of this writing: 12 distinct fixes were needed to get from "cannot parse real CAM at all" to "parses real CAM and CDD with zero lexer/parser errors," spanning:

- Stub/unimplemented rules (`assignedIdentifier` was a literal empty production — never parsed import module OIDs at all).
- Incomplete alternatives already scaffolded but commented out (nested constraint grouping, `objectSetElements`, `WITH COMPONENT` singular).
- A lexer/parser token-boundary bug: the lexer's `EXTENSTIONENDMARKER` rule greedily fuses `,` and `...` into one token when written without whitespace (`,...`, normal ASN.1 style), silently breaking five separate `COMMA ELLIPSIS` parser alternatives that could then never match.
- Missing constructs with no prior support at all: explicit `[N]` type tagging, and inline Information Object literals (`{Type WORD BY value}` — the common idiom, not full generic X.681 WITH-SYNTAX-driven object parsing, see §8).
- One straightforward copy-paste bug (`sequenceOfType` required an extra, spec-incorrect pair of parens its sibling `setOfType` rule correctly did not).

Renamed on fork (`AsnEtsiItsLexer`/`AsnEtsiItsParser`) since, after that many substantive changes, keeping the upstream name would misrepresent it as unmodified — see `compiler/grammar/README.md`.

### 3.2 Toolchain: Java 11+ for codegen, C++17 + ANTLR4 C++ runtime for the built parser

- Regenerating `.cpp`/`.h` from `.g4` needs a JRE 11+ (ANTLR 4.13.2's tool jar itself requires class file version 55; a JRE 8 cannot run it) — this is a **build-time-only, human-invoked step**, not part of `compiler/`'s CMake build. A portable JRE (no installer) is sufficient; a full JDK/`javac` is never needed since only the Python/C++/etc. *targets* are generated, not compiled Java.
- The built parser links the ANTLR4 C++ runtime, fetched from source and built via CMake `FetchContent` (`compiler/CMakeLists.txt`), matching `runtime/`'s existing GoogleTest `FetchContent` pattern. One real upstream bug was found and patched at fetch time (`compiler/cmake/patch_antlr4_chrono.cmake`, idempotent): `ProfilingATNSimulator.cpp` uses `std::chrono::high_resolution_clock` without `#include <chrono>`, relying on transitive inclusion that does not hold under newer/stricter STL implementations (hit under MSVC 14.5x / VS 2026). Not yet reported upstream.

### 3.3 A real, separate bug found in the ETSI source itself

`ETSI-ITS-CDD.asn` (TS 102 894-2, fetched from `forge.etsi.org`) contains ~34 bytes of stray Windows-1252 encoding (e.g. a smart-quote apostrophe at line 570) in an otherwise-UTF-8 file — a genuine mistake in ETSI's own published file, not something introduced here. Feeding it raw crashes (not just errors) the ANTLR4 C++ runtime's UTF-8 decoder — `STATUS_STACK_BUFFER_OVERRUN`, reproduced with an oversized stack to rule out simple stack exhaustion first. Worked around by re-encoding to valid UTF-8 before vendoring (`compiler/tests/fixtures/`); the crash-vs-error behavior itself is arguably a second real ANTLR4 C++ runtime robustness gap, not pursued further here.

---

## 4. Current State (updated 2026-08-15)

`compiler/grammar/` parses the **full core ETSI ITS message set** — CAM, CDD, DENM, CPM, VAM, and MAPEM/SPATEM/IVIM/SREM/SSEM/RTCMEM (19 real files total, see `compiler/tests/fixtures/`) — with **zero lexer/parser errors**, verified via `AsnParserSmokeTest` (CTest, wired into the root build, recursively walks `fixtures/`). Notably: all 12 fixes made against CAM+CDD alone (§3.1) generalized to the other 17 files with **zero additional grammar changes needed** — the common X.680/X.681 machinery those fixes covered (constraint syntax, extensibility, tagging, OID forms, the Information Object idiom) turned out to be genuinely complete for this message set, not just for CAM/CDD specifically.

**Since then, §6's Visitor-based extraction proof is also done**: the parse tree's shape is now confirmed to be a workable basis for structured extraction, not just syntax — see §6 for the AST that was built and what it does/doesn't prove.

**Explicitly not yet done, and not to be conflated with the above:**
- No semantic IR exists. No semantic analysis (cross-module import resolution, AUTOMATIC TAGS computation, constraint-to-`SizeRange` translation) exists.
- No codegen exists; nothing from this front end has yet produced a single line of code calling into `runtime/`.

---

## 5. Message Set Coverage

**Full core set validated as of 2026-08-09** — CAM, CDD, DENM (TS 103 831), CPM (TS 103 324), VAM (TS 103 300-3), and the TS 103 301 infrastructure-message repo (MAPEM/SPATEM/IVIM/SREM/SSEM/RTCMEM, plus the DSRC/region modules they import from). 19 real files total, all pulled from `forge.etsi.org/rep/ITS/asn1/`, all parsing clean. CPM was flagged in the prior revision of this document as a specific risk (cooperative-perception-specific container patterns, untested against this grammar) — it parsed clean along with everything else, no new gap materialized.

**Not included, deliberately:** MCM (TS 103 561) — still a draft standard, no stable ASN.1 module published on the forge as of this writing (confirmed by listing the full `ITS/asn1` group: no MCM project exists there). Revisit once ETSI publishes one.

With this, message-set breadth is no longer the open risk for this front end — §6 and §8 (parse-tree soundness, `asn1c` availability) are.

---

## 6. Tree Walking and AST Construction (done, 2026-08-15 — not the semantic IR)

ANTLR4 builds a **concrete** syntax tree (every grammar rule is a node, including punctuation and — as seen while patching this grammar — real depth: a single constraint value nests seven rules deep, `elements → subtypeElements → value → builtinValue → integerValue → signedNumber → NUMBER`). Two built-in traversal mechanisms:

- **Listener** (always generated): ANTLR walks the tree, calling `void enterX`/`exitX` callbacks. Building an IR this way means threading mutable state between enter/exit calls to pass data up — awkward for anything beyond linear accumulation.
- **Visitor** (generated via `-visitor`, already present in `compiler/src/generated/`): traversal is driven explicitly, and each `visitX` method **returns a value** — `visitAsnType` can return an `IR::Type`, `visitComponentType` an `IR::Field` built from its visited children. Maps directly onto bottom-up IR construction.

**Decision: Visitor.** No extra setup cost (already codegen'd with `-visitor`). `AstBuilder` (`compiler/src/ast_builder.h`/`.cpp`) subclasses the generated `AsnEtsiItsParserBaseVisitor`, but in practice most of its methods are called directly by concrete `XyzContext*` type rather than routed through the generic `accept()`/`std::any` dispatch — the grammar already statically determines the next rule at almost every call site (e.g. a `ComponentTypeContext*` always yields a `Field`), so the indirection buys nothing there. `accept()`/`std::any` would only earn its keep at the genuinely polymorphic points (`builtinType`'s alternatives, `assignment`'s four alternatives), and even those are handled with plain `if`-chains over which child accessor is non-null, which is both simpler and something the grammar already guarantees is mutually exclusive.

**The validation step this section previously called out as not-yet-done is done**: a Visitor-based extraction proof against the real CAM parse tree, using `CamParameters` (optional fields, extensibility, cross-module-referenced containers) as the named test case, exactly as planned. Result: the CST *is* a sound basis for structured extraction — `compiler/tests/ast_extraction_test.cpp` walks all 19 real fixtures through `AstBuilder` with zero crashes, and asserts the full expected shape of `CamParameters` (5 fields, correct optional/extension-addition flags, correct referenced-type names) and `HighFrequencyContainer` (a CHOICE, 2 root alternatives) out of the real CAM file. Wired into CTest as `AsnAstExtractionTest`.

**The AST produced (`compiler/include/v2x/asn/ast.hpp`) is deliberately encoding-rule-agnostic**, checked explicitly against COER and BER, not designed against PER alone — `runtime/` today only targets UPER and COER/BER/1609.2 are formally deferred (§8 below, STATUS.md Phase 4), but this AST is meant to be the one front end any future codegen sits on top of. Two decisions follow from that check:

- **Constraints are captured as structured min/max bounds, not raw text** (`Constraint{kind, rootRanges, extensionRanges, extensible, rawText}` — a `Bound` is a literal, `MIN`/`MAX`, or a named/defined-value reference). This is exactly the deepest CST chain called out above, walked structurally rather than punted to `getText()` — punting would have sidestepped the one thing this validation step needed to prove. Grounded in what the 19 real fixtures actually contain (grepped, not guessed): 123 `SIZE(...)` occurrences (plain and extensible, occasionally two ranges e.g. `SIZE(1..16,...,17..40)`), plain INTEGER value ranges, and ~30 `WITH COMPONENTS {...}` presence constraints (a validation-level concern, not wire-shape). The extraction walks `constraint → constraintSpec → subtypeConstraint → elementSetSpecs → ... → subtypeElements` and recognizes plain range/size shapes; anything else (real unions/intersections of more than one term, `ALL EXCEPT`, `WITH COMPONENT(S)`, `PATTERN`, `generalConstraint`) degrades to `Raw` (rawText only) rather than guessing. This structured form is meaningful to both PER's bit-packing and COER's octet-packing of constrained values — same input data, different downstream packing per encoding rule, which is exactly the right layering.
- **Tags are captured as structured class/number/mode** (`Tag{tagClass, number, mode}`), not raw text, and `Module` captures its `tagDefault`/`extensionDefault`. PER mostly ignores tags, so this was easy to under-scope; BER/DER is fundamentally tag-driven TLV, so an AST that only serves PER well would have been a real gap here. Actually *computing* AUTOMATIC TAGS (assigning positional tags when unspecified) is unaffected by this — still explicit future work below — this only stops the raw facts from being silently discarded.

**Still explicitly not done, unaffected by the above:** cross-module import resolution (imports are captured as symbol/module-name pairs, not resolved), AUTOMATIC TAGS computation, constraint-to-`SizeRange` translation for codegen, and the X.681 Information Object System / parameterized types (`CLASS`/`objectClassAssignment` and `parameterizedAssignment` are recorded as an opaque `UnsupportedAssignment{name, reason}` rather than modeled or crashed on — real, already-known gaps per §8, not newly introduced here).

---

## 7. Safety/MISRA Posture Boundary (`runtime/` vs. this front end)

`per-runtime-design.md` §7 commits `runtime/` to a MISRA-C++-informed, no-exceptions/no-heap/no-RTTI/bounded-execution posture because it runs on-target, processing untrusted wire data in a vehicle's safety path. **This front end does not, and should not, share that posture** — it is host-side tooling, never shipped, and the ANTLR4 C++ runtime it depends on uses exceptions, heap allocation, and recursion throughout.

This is a deliberate position, reasoned through explicitly (not an oversight to revisit casually): a code generator producing safety-relevant source code is still part of the supply chain — this is what ISO 26262 Part 8's Tool Confidence Level concept is about. The accepted industry answer is not "the generator itself must be MISRA-C" (none of the real ASN.1 compilers used in automotive — `asn1c`, OSS Nokalva's, Objective Systems' — are), it's **establishing confidence in the tool's *output* through independent verification.** Concretely here: `per-runtime-design.md` §10's planned Layer 4 (differential testing against `asn1c` — same real message, both toolchains, byte-diff the output) is not optional polish once codegen exists; it is the actual mitigation for this front end's dependency on a non-MISRA toolchain, and should be treated as load-bearing, not deferred.

---

## 8. Explicit Deferrals / Known Gaps

- **Full generic X.681 Information Object System (WITH-SYNTAX-driven object parsing)** — not implemented. Only the common `{Type WORD BY value}` idiom (`object_`/`objectDefn` in the grammar) is supported, which covered CAM's one real use (`ExtensionContainers`). A module leaning harder on custom `WITH SYNTAX` notation would need this generalized.
- **Recursion/cycle detection in the type-reference graph** — attempted via a text-based heuristic against real CAM/CDD, found unreliable (false positives from ASN.1 doc-comment cross-references — see `per-runtime-design.md` §9). A real check needs the validated parse tree, not text. Not yet built.
- **`asn1c` toolchain availability for Layer 4** — checked at the end of the 2026-08-08 session: no native `gcc`/`make`/`bison`/`flex` on this machine; WSL is present but distro/toolchain readiness unconfirmed. Open item before Layer 4 differential testing can start.
- **IR design itself** — not started. §6 records the traversal-mechanism decision and the recommended next validation step, not an IR shape.

---

*This is a living draft, expected to grow substantially once IR design and semantic analysis begin — at that point this document should absorb that design rather than spinning off a third document, per the scope note at the top.*
