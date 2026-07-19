**Last reviewed: 2026-07-19**

# ETSI ITS Facilities-Layer Message Reference

Quick-reference index of the message set. See [`flows.md`](flows.md) for sequence diagrams of how CAM and DENM actually get triggered and propagated.

| Message | Full name | Trigger model | Typical interface | Core spec | ASN.1 module |
|---|---|---|---|---|---|
| **CAM** | Cooperative Awareness Message | Periodic (1–10 Hz, dynamically triggered by heading/speed/position delta thresholds) | PC5 | TS 103 900 (R2) / EN 302 637-2 (R1) | `cam_ts103900` |
| **DENM** | Decentralized Environmental Notification Message | Event-triggered (hazard detected), repeated until cancelled/expired | PC5 + Uu relay | TS 103 831 (R2) / EN 302 637-3 (R1) | `denm` |
| **CPM** | Collective Perception Message | Periodic, content-driven by sensed-object set changes | PC5 | TS 103 324 | `cpm_ts103324` |
| **VAM** | VRU Awareness Message | Periodic, CAM-equivalent for vulnerable road users | PC5 | TS 103 300-3 | `vam-ts103300_3` |
| **MAPEM** | Map (topology) Extended Message | Periodic, static/semi-static per intersection | PC5 (local) / Uu (wide-area) | TS 103 301 | `is_ts103301` |
| **SPATEM** | Signal Phase and Timing Extended Message | Periodic, tied to signal controller state changes | PC5 (local) | TS 103 301 | `is_ts103301` |
| **IVIM** | In-Vehicle Information Message | Event/context-triggered (roadworks, VMS content changes) | PC5 / Uu | TS 103 301 | `is_ts103301` |
| **SREM / SSEM** | Signal Request / Status Extended Message | On-demand (priority request/response) | PC5 (local) | TS 103 301 | `is_ts103301` |
| **RTCMEM** | RTCM Correction Message | Periodic, GNSS correction relay | PC5 / Uu | TS 103 301 | `is_ts103301` |
| **MCM** | Manoeuvre Coordination Message | Event/context-triggered (negotiated manoeuvre) — **still an ongoing standard, not finalized** as of early 2026 | PC5 | TS 103 561 (draft) | not yet in stable ASN.1 release |

## Reading this table

- **Trigger model** matters more than it looks — it's the difference between "beacon" messages (CAM/CPM/VAM/MAPEM/SPATEM: sent continuously regardless of anything happening) and "notification" messages (DENM/IVIM/MCM: sent because something specific occurred). This distinction drives very different encoder logic and testing strategy — periodic messages need cadence/congestion-control testing, event messages need trigger-condition and lifecycle (repeat/cancel/expire) testing.
- **ASN.1 module** links back to the [Hitchhiker's Guide §7](../hitchhikers-guide.md#7-the-ultimate-answer--core-asn1-repository) and the [`forge.etsi.org/rep/ITS/asn1`](https://forge.etsi.org/rep/ITS/asn1) repo structure.
- MCM is flagged separately since it's the one message in this table that isn't in a stable release yet — worth not treating it as equivalent-maturity to the others in any implementation work.
