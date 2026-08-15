# compiler/

Phase 2 — ANTLR-based ASN.1 front end, IR, and codegen targeting `runtime/`.

**Grammar/parser front end: started, real-file-validated.** `grammar/` has an
ANTLR4 grammar (`AsnEtsiItsLexer.g4`/`AsnEtsiItsParser.g4`, forked from
`antlr/grammars-v4`'s `asn_3gpp`) that parses real ETSI CAM (TS 103 900) and
CDD (TS 102 894-2) modules with zero lexer/parser errors — see
`tests/fixtures/` for the actual files and `tests/parser_smoke_test.cpp` for
the check, wired into CTest as `AsnParserSmokeTest`.

**AST layer: also built and validated.** `include/v2x/asn/ast.hpp` +
`src/ast_builder.h`/`.cpp` — a Visitor-based (`AstBuilder`, subclasses the
generated `AsnEtsiItsParserBaseVisitor`) extraction of a flattened AST from
the parse tree, proving the tree is actually a workable basis for pulling
structured type/field/constraint data back out (not just syntax-clean).
`tests/ast_extraction_test.cpp` (`AsnAstExtractionTest` in CTest) walks all
19 real fixtures through it with zero crashes, and asserts the full expected
shape of `CamParameters` and `HighFrequencyContainer` out of the real CAM
file, plus structured constraint extraction against two real CDD types. See
`docs/design/compiler-frontend-design.md` §6 for the design (notably: why
constraints are captured as structured ranges rather than raw text, and why
tags are structured too — both checked explicitly against future COER/BER
codegen, not just PER).

**What this still doesn't prove:** the AST above is not the semantic IR —
no cross-module import resolution, no AUTOMATIC TAGS computation, no
constraint-to-`SizeRange` translation, no codegen. `CLASS`/parameterized-type
assignments (the X.681 Information Object System) are recorded as opaque
"unsupported" rather than modeled.

IR design, type resolution/import handling, and codegen targeting `runtime/`
are all not started. `runtime/`'s own fuzz-stability work (Layer 5, see
`../STATUS.md`) is also still not started — this grammar work proceeded in
parallel rather than waiting on it, per explicit direction, but the codegen
phase should still not assume the runtime kernel underneath is fuzz-clean.
