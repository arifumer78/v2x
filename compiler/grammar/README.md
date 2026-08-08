# grammar/

`AsnEtsiItsLexer.g4` / `AsnEtsiItsParser.g4` — an ANTLR4 grammar for ETSI ITS
ASN.1 modules, forked from [antlr/grammars-v4's `asn_3gpp`
grammar](https://github.com/antlr/grammars-v4/tree/master/asn/asn_3gpp).
Renamed on fork since it is no longer the unmodified upstream grammar — see
the header comment in `AsnEtsiItsParser.g4` for the full list of changes and
why each was needed. Upstream could not parse real ETSI CAM (TS 103 900) or
CDD (TS 102 894-2) modules at all; this fork parses both with zero
lexer/parser errors (see `../tests/`).

`AsnEtsiItsLexerBase.h` is the lexer's C++ superclass (one helper predicate,
`IsColumnZero()`, used by the line-comment rule).

## Regenerating the generated C++ sources

The generated parser C++ (`../src/generated/`) is committed to the repo, not
regenerated at build time — building `compiler/` needs only a C++ toolchain,
not Java. If you edit the `.g4` files, regenerate manually:

1. A JRE 11+ is required to run the ANTLR tool jar itself (it will not run
   on Java 8). Get the latest ANTLR complete jar from
   https://www.antlr.org/download.html.
2. From this directory:
   ```
   java -jar antlr-4.13.2-complete.jar -Dlanguage=Cpp -visitor AsnEtsiItsLexer.g4 AsnEtsiItsParser.g4
   ```
3. Copy the generated `.cpp`/`.h` files into `../src/generated/`, replacing
   the existing ones.
4. Rebuild and rerun `AsnParserSmokeTest` (via `ctest` from the build
   directory) to confirm both real ETSI fixtures still parse clean.
