# Changelog

Dated, terse entries — enough to answer "what changed since I last looked," not a full commit log.

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
