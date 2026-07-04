# v2x

A place to study, build, and share ETSI V2X (C-V2X) tooling — starting with a reference guide to the ecosystem, and building toward a from-scratch, safety-conscious ASN.1/PER codec for the ETSI ITS facilities-layer message set.

## Start here

- **[Hitchhiker's Guide to ETSI C-V2X](docs/hitchhikers-guide.md)** — what C-V2X is, the ETSI TC ITS working group structure, active projects (Plugtests, C-ROADS), where the value-add opportunities are, and the core ASN.1 repository.
- **[PER Runtime Design Document](docs/design/per-runtime-design.md)** — the design for the X.691 PER codec kernel (Phase 1 of the build).
- **[STATUS](STATUS.md)** — what's actually done vs. planned, updated per work session.
- **[CHANGELOG](docs/CHANGELOG.md)** — dated log of what changed and why.

## Repo layout

```
docs/            reference material and design docs
runtime/         Phase 1 — hand-built, fuzz-tested X.691 PER codec kernel
compiler/        Phase 2 — ANTLR-based ASN.1 front end, IR, codegen (built on runtime/)
tools/           Phase 3 — visualizers, RSU simulators, cloud/MEC adapters
```

Nothing under `runtime/`, `compiler/`, or `tools/` exists yet beyond scaffolding — see STATUS.md for the honest current state.
