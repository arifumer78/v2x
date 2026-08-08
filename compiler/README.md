# compiler/

Phase 2 — ANTLR-based ASN.1 front end, IR, and codegen targeting `runtime/`.

**Grammar/parser front end: started, real-file-validated.** `grammar/` has an
ANTLR4 grammar (`AsnEtsiItsLexer.g4`/`AsnEtsiItsParser.g4`, forked from
`antlr/grammars-v4`'s `asn_3gpp`) that parses real ETSI CAM (TS 103 900) and
CDD (TS 102 894-2) modules with zero lexer/parser errors — see
`tests/fixtures/` for the actual files and `tests/parser_smoke_test.cpp` for
the check, wired into CTest as `AsnParserSmokeTest`.

**Important caveat:** this only proves the grammar consumes real ETSI syntax
without errors. It does not yet prove the resulting parse tree is a sound
basis for IR generation — that needs a Visitor-based extraction test (pull
real type/field/constraint data out of the tree) before trusting this as the
actual compiler front end. Not done yet.

IR design, type resolution/import handling, and codegen targeting `runtime/`
are all not started. `runtime/`'s own fuzz-stability work (Layer 5, see
`../STATUS.md`) is also still not started — this grammar work proceeded in
parallel rather than waiting on it, per explicit direction, but the codegen
phase should still not assume the runtime kernel underneath is fuzz-clean.
