# v2x

A place to study, build, and share **ETSI C-V2X (Cellular Vehicle-to-Everything)** tooling — a reference guide to the ecosystem, plus a from-scratch, safety-conscious **ASN.1 / X.691 UPER (Unaligned Packed Encoding Rules)** codec for the **ETSI Intelligent Transport Systems (ITS)** facilities-layer message set (**CAM**, **DENM**, **CPM**, **VAM**, and related messages).

## Start here

- **[Hitchhiker's Guide to ETSI C-V2X](docs/hitchhikers-guide.md)** — what C-V2X is, the ETSI TC ITS working group structure, active projects (Plugtests, C-ROADS), where the value-add opportunities are, and the core ASN.1 repository.
- **[PC5 vs Uu](docs/architecture/pc5-vs-uu.md)** — the two C-V2X radio interfaces compared, with a topology diagram.
- **[Message reference](docs/messages/README.md)** — facilities-layer message index (CAM, DENM, CPM, VAM, MAPEM/SPATEM/IVIM/SREM/SSEM/RTCMEM, MCM) with trigger models, specs, and ASN.1 modules; see also the [CAM/DENM flow diagrams](docs/messages/flows.md).
- **[PER Runtime Design Document](docs/design/per-runtime-design.md)** — the design for the UPER/X.691 codec kernel.
- **[STATUS](STATUS.md)** — what's actually done vs. planned, updated per work session.
- **[CHANGELOG](docs/CHANGELOG.md)** — dated log of what changed and why.

## Repo layout

```
docs/            reference material and design docs
runtime/         hand-built, ASan-verified UPER (X.691 Unaligned Packed Encoding Rules) codec
                 kernel + a generic ASN.1 orchestration layer (SEQUENCE, CHOICE, SEQUENCE OF,
                 ENUMERATED, OCTET STRING, BIT STRING) — 151 tests, see STATUS.md
compiler/        Phase 2 — ANTLR-based ASN.1 (.asn) parser front end, IR, codegen (not started)
tools/           Phase 3 — visualizers, RSU simulators, cloud/MEC adapters (not started)
```

See [STATUS.md](STATUS.md) for the honest, current state of each piece.
